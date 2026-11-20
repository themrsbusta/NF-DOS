# NF-DOS 6.0

NF-DOS 6.0 is the final product release and the public product name.

## Downloads

The GitHub Release provides the bootable images and the identified source archive:

- [Bootable CD image](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS6.0.iso)
- [1.44 MB floppy image](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS6.0.img)
- [Source archive](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS-6.0-source.zip)
- [SHA-256 checksums](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/SHA256SUMS.txt)

The source archive is also stored at `SOURCE/NF-DOS-6.0-source.zip`. Its scope and checksum are documented in `DOCS/NF-DOS_60_Release.yaml` and `DOCS/SHA256SUMS.txt`. Download the two image assets into `RELEASES/` before checking all package hashes.

## Validation and scope

The release handoff records that the ISO booted in QEMU 11.1 to the NF-DOS 6.0 splash screen and DOS prompt. The `.IMG` and `.ISO` files are distributed as release assets rather than Git blobs.

The source archive contains identified source, patches, documentation, and notices. It is not a complete corresponding-source package for every application on the ISO, and it is not a byte-for-byte reproducible build recipe. The ISO has not received the same detailed technical audit as the floppy image. `DISTRIBUTION-COMPLIANCE.md` lists the unresolved source-to-binary, Open Watcom, and third-party redistribution questions. The publication of these files does not state that those questions have been cleared.

## Licensing

The MIT license covers only the Panthera-authored material identified in `LICENSE-SCOPE.md`. Third-party components retain their own licenses and notices. The source archive preserves those terms and attribution.
