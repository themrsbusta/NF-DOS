from __future__ import annotations

import argparse
import hashlib
import struct
import subprocess
import zlib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
GENERATED = HERE / "generated"
DEFAULT_IMAGE = HERE / "reference" / "def-splash.png"

# The 711x565 source is reduced without filtering to a 50x40 bitmap.
# Each pair of bitmap rows is shown by one VGA text cell using CP437 blocks.
IMAGE_WIDTH, IMAGE_HEIGHT = 50, 40
SCREEN_COLUMNS, SCREEN_ROWS = 80, 20
TOTAL_TEXT_ROWS = 25
ART_LEFT = (SCREEN_COLUMNS - IMAGE_WIDTH) // 2
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"


def paeth(a: int, b: int, c: int) -> int:
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def decode_png(path: Path) -> tuple[int, int, list[bytearray]]:
    """Decode the supplied non-interlaced, 8-bit RGB PNG using stdlib only."""
    data = path.read_bytes()
    if not data.startswith(PNG_SIGNATURE):
        raise ValueError(f"not a PNG file: {path}")

    offset = len(PNG_SIGNATURE)
    width = height = bit_depth = color_type = interlace = None
    compressed = bytearray()
    saw_end = False
    while offset < len(data):
        if offset + 12 > len(data):
            raise ValueError("truncated PNG chunk")
        length = struct.unpack_from(">I", data, offset)[0]
        tag = data[offset + 4:offset + 8]
        start = offset + 8
        end = start + length
        if end + 4 > len(data):
            raise ValueError("truncated PNG payload")
        payload = data[start:end]
        expected_crc = struct.unpack_from(">I", data, end)[0]
        if zlib.crc32(tag + payload) & 0xFFFFFFFF != expected_crc:
            raise ValueError(f"PNG CRC failure in {tag!r}")
        if tag == b"IHDR":
            if length != 13:
                raise ValueError("bad PNG IHDR length")
            width, height, bit_depth, color_type, compression, filt, interlace = struct.unpack(
                ">IIBBBBB", payload
            )
            if compression != 0 or filt != 0:
                raise ValueError("unsupported PNG compression/filter method")
        elif tag == b"IDAT":
            compressed.extend(payload)
        elif tag == b"IEND":
            saw_end = True
            break
        offset = end + 4

    if not saw_end or width is None or height is None:
        raise ValueError("PNG is missing IHDR or IEND")
    if (bit_depth, color_type, interlace) != (8, 2, 0):
        raise ValueError("expected a non-interlaced, 8-bit RGB PNG")

    stride = width * 3
    raw = zlib.decompress(compressed)
    if len(raw) != height * (stride + 1):
        raise ValueError("PNG decompressed data has an unexpected size")

    rows: list[bytearray] = []
    prior = bytearray(stride)
    cursor = 0
    for _ in range(height):
        filter_type = raw[cursor]
        cursor += 1
        scan = raw[cursor:cursor + stride]
        cursor += stride
        recon = bytearray(stride)
        for i, value in enumerate(scan):
            left = recon[i - 3] if i >= 3 else 0
            up = prior[i]
            upper_left = prior[i - 3] if i >= 3 else 0
            if filter_type == 0:
                predictor = 0
            elif filter_type == 1:
                predictor = left
            elif filter_type == 2:
                predictor = up
            elif filter_type == 3:
                predictor = (left + up) // 2
            elif filter_type == 4:
                predictor = paeth(left, up, upper_left)
            else:
                raise ValueError(f"unsupported PNG row filter {filter_type}")
            recon[i] = (value + predictor) & 0xFF
        rows.append(recon)
        prior = recon
    return width, height, rows


def monochrome_mask(rows: list[bytearray], width: int) -> list[list[int]]:
    mask: list[list[int]] = []
    for row in rows:
        bits = []
        for x in range(width):
            red, green, blue = row[x * 3:x * 3 + 3]
            if max(red, green, blue) - min(red, green, blue) > 8:
                raise ValueError("reference logo contains unexpected color pixels")
            # The supplied PNG has a small antialiased fringe. Threshold it once
            # to the DOS renderer's two colors; resizing itself remains nearest-neighbor.
            luminance = (299 * red + 587 * green + 114 * blue) // 1000
            bits.append(1 if luminance >= 128 else 0)
        mask.append(bits)
    return mask


def resize_nearest(mask: list[list[int]], out_width: int, out_height: int) -> list[list[int]]:
    source_height = len(mask)
    source_width = len(mask[0])
    return [
        [
            mask[min(source_height - 1, ((2 * y + 1) * source_height) // (2 * out_height))]
            [min(source_width - 1, ((2 * x + 1) * source_width) // (2 * out_width))]
            for x in range(out_width)
        ]
        for y in range(out_height)
    ]


def png_chunk(tag: bytes, payload: bytes) -> bytes:
    return (
        struct.pack(">I", len(payload))
        + tag
        + payload
        + struct.pack(">I", zlib.crc32(tag + payload) & 0xFFFFFFFF)
    )


def write_rgb_png(width: int, height: int, pixels: bytes, path: Path) -> None:
    stride = width * 3
    raw = b"".join(b"\0" + pixels[y * stride:(y + 1) * stride] for y in range(height))
    output = bytearray(PNG_SIGNATURE)
    output.extend(png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)))
    output.extend(png_chunk(b"IDAT", zlib.compress(raw, 9)))
    output.extend(png_chunk(b"IEND", b""))
    path.write_bytes(output)


def screen_mask(mask: list[list[int]]) -> list[list[int]]:
    result = [[0] * SCREEN_COLUMNS for _ in range(TOTAL_TEXT_ROWS * 2)]
    for y, row in enumerate(mask):
        for x, white in enumerate(row):
            result[y][ART_LEFT + x] = white
    return result


def make_screen(mask: list[list[int]]) -> bytes:
    pixels = screen_mask(mask)
    result = bytearray()
    for cell_row in range(SCREEN_ROWS):
        for col in range(SCREEN_COLUMNS):
            top = pixels[cell_row * 2][col]
            bottom = pixels[cell_row * 2 + 1][col]
            if top and bottom:
                char = 0xDB  # CP437 full block
            elif top:
                char = 0xDF  # CP437 upper half block
            elif bottom:
                char = 0xDC  # CP437 lower half block
            else:
                char = 0x20
            attr = 0x0F if top or bottom else 0x07
            result.extend((char, attr))
    expected = SCREEN_COLUMNS * SCREEN_ROWS * 2
    if len(result) != expected:
        raise ValueError(f"screen buffer is {len(result)} bytes, expected {expected}")
    return bytes(result)


def write_preview(mask: list[list[int]], path: Path) -> None:
    # One logical half-block pixel is 8x8 physical pixels in 80x25 text mode.
    canvas_width = SCREEN_COLUMNS * 8
    canvas_height = TOTAL_TEXT_ROWS * 16
    rgb = bytearray(canvas_width * canvas_height * 3)
    pixels = screen_mask(mask)
    for y, row in enumerate(pixels):
        for x, white in enumerate(row):
            if white:
                for py in range(y * 8, (y + 1) * 8):
                    start = (py * canvas_width + x * 8) * 3
                    rgb[start:start + 24] = b"\xff" * 24
    write_rgb_png(canvas_width, canvas_height, bytes(rgb), path)


def write_svg(mask: list[list[int]], path: Path) -> None:
    pieces = [
        '<svg xmlns="http://www.w3.org/2000/svg" width="640" height="400" viewBox="0 0 80 50">',
        '<rect width="80" height="50" fill="#000"/>',
    ]
    pixels = screen_mask(mask)
    for y, row in enumerate(pixels):
        x = 0
        while x < SCREEN_COLUMNS:
            if not row[x]:
                x += 1
                continue
            start = x
            while x < SCREEN_COLUMNS and row[x]:
                x += 1
            pieces.append(f'<rect x="{start}" y="{y}" width="{x-start}" height="1" fill="#fff"/>')
    pieces.append("</svg>")
    path.write_text("\n".join(pieces) + "\n", encoding="ascii")


def main() -> None:
    parser = argparse.ArgumentParser(description="Render the supplied NF-DOS PNG into NFSPLASH's VGA text page.")
    parser.add_argument("--image", type=Path, default=DEFAULT_IMAGE)
    parser.add_argument("--nasm", type=Path, default=ROOT / "tools" / "nasm" / "nasm-3.02" / "nasm.exe")
    parser.add_argument("--output", type=Path, default=GENERATED / "NFSPLASH.COM")
    args = parser.parse_args()

    if not args.image.is_file():
        raise SystemExit(f"Reference image is missing: {args.image}")
    if not args.nasm.is_file():
        raise SystemExit(f"NASM is missing: {args.nasm}")

    source_width, source_height, source_rows = decode_png(args.image)
    source_mask = monochrome_mask(source_rows, source_width)
    source_ratio = source_width / source_height
    target_ratio = IMAGE_WIDTH / IMAGE_HEIGHT
    if abs(target_ratio / source_ratio - 1.0) > 0.01:
        raise ValueError("chosen text-mode dimensions distort the source aspect ratio by more than 1%")

    mask = resize_nearest(source_mask, IMAGE_WIDTH, IMAGE_HEIGHT)
    GENERATED.mkdir(parents=True, exist_ok=True)
    (GENERATED / "logo-50x40.bin").write_bytes(bytes(bit for row in mask for bit in row))
    write_preview(mask, GENERATED / "logo-preview.png")
    write_svg(mask, GENERATED / "logo.svg")
    screen = make_screen(mask)
    (GENERATED / "screen.bin").write_bytes(screen)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(
        [str(args.nasm), "-f", "bin", str(HERE / "NFSPLASH.ASM"), "-o", str(args.output)],
        cwd=HERE,
        check=True,
    )
    com = args.output.read_bytes()
    if len(com) >= 0x10000 or com.startswith(b"MZ"):
        raise SystemExit("NFSPLASH.COM is not a valid small DOS .COM binary")

    digest = hashlib.sha256(args.image.read_bytes()).hexdigest().upper()
    aspect_delta = abs(target_ratio / source_ratio - 1.0) * 100
    print(
        f"source={source_width}x{source_height} sha256={digest}; "
        f"logo={IMAGE_WIDTH}x{IMAGE_HEIGHT} nearest-neighbor centered; "
        f"aspect_delta={aspect_delta:.3f}%; screen={len(screen)} bytes; "
        f"COM={len(com)} bytes; output={args.output}"
    )


if __name__ == "__main__":
    main()
