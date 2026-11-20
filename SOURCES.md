# Sources and provenance

This repository includes source snapshots and documented patches used for NF-DOS 6.0. Directories preserve the license and notice files tracked by each project. Disk images, compiled output, and nested `.git` directories are not included.

| Component | Repository | Source commit |
|---|---|---|
| FreeDOS Kernel | [FDOS/kernel](https://github.com/FDOS/kernel) | `5ffb5502d39a10a30f5b8a9e8beeba0bf30245d3` |
| Country tables | [FDOS/country](https://github.com/FDOS/country) | `7f83e041d00f78b3912c761246930f3b437440f6` |
| FreeCOM | [FDOS/freecom](https://github.com/FDOS/freecom) | `d52ba900a5e5b3ab29e0bba0735e6b8f076d2b53` |
| FDISK | [FDOS/fdisk](https://github.com/FDOS/fdisk) | `a6e3622c959707f1950df8ca4fe94046274ee1a9` |
| ATTRIB | [FDOS/attrib](https://github.com/FDOS/attrib) | `20e373d2fcef5e6369075debcec0fceecc9f9493` |
| CHKDSK | [FDOS/chkdsk](https://github.com/FDOS/chkdsk) | `827a4c62a0dbe268e3443deb0e626f04c2818c14` |
| DEBUG | [FDOS/debug](https://github.com/FDOS/debug) | `f73c7858cd2d4d470c09fce78645f0c9ac5ff047` |
| EDIT | [FDOS/edit](https://github.com/FDOS/edit) | `4ed5d50a4d2d516e857478ab437985d1e07b9eb3` |
| FORMAT | [FDOS/format](https://github.com/FDOS/format) | `9d19403695916f98a121b7deeb0a6cdc92d29144` |
| SYS | [FDOS/sys](https://github.com/FDOS/sys) | `33e93c659d59c607d3fcbbed3d7561b04b856010` |
| FDAPM | [FDOS/fdapm](https://github.com/FDOS/fdapm) | `61bf59098c24f77cb3284be6e0ee770e82c14daa` |
| HIMEMX | [FDOS/himemX](https://github.com/FDOS/himemX) | `f5f2314d64cd0e59b4062e53f9c1e30bbe0d2c9b` |
| BITDISK / ramdisk source | [FDOS/ramdisk](https://github.com/FDOS/ramdisk) | `598749ea97e2cfaea71e249215c4d74ff8f58885` |
| CuteMouse source | [FDOS/mouse](https://github.com/FDOS/mouse) | `dd268521dc76d323d0597d1b21f58be5b82c6c25` |
| SHSUCDX | [adoxa/shsucd](https://github.com/adoxa/shsucd) | `ad7f92020f3d026fe658864578481f9bbb953181` |
| ZIP | [cysin/info-zip](https://github.com/cysin/info-zip) | `b4b2ab3c5ae0dce0a7c3884f364ad1fd15854d90` |
| UnZip | [madler/unzip](https://github.com/madler/unzip) | `0b82c20ac7375b522215b567174f370be89a4b12` |
| CABExtract 1.11+1 | [FreeDOS/cabext](https://gitlab.com/FreeDOS/archiver/cabext), upstream reference [dajhorn/libmspack](https://github.com/dajhorn/libmspack), package `2025-03-16.0` | `51b1ea8b329945c4c1756c41abdfe26487a6b2f2` |
| ATAPICDD 0.2.14 | [FreeDOS/atapicdd](https://gitlab.com/FreeDOS/drivers/atapicdd), package `2025-04-10.0` | `bbb748e82d5b4b5885a65f0c2372a9c622d1de2b` |
| SRDISK 2.09d | [FreeDOS 1.4 SRDISK package](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/1.4/drivers/srdisk.zip) | Source and notices only in `third_party/srdisk/`; compiled `BIN/` files excluded |
| DOSFSCK 2.11d | [FreeDOS package catalog](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/disk/dosfsck/20250409.8/index.html) | Package source snapshot `2025-04-09.8`; no Git commit recorded. The image executable matches the official package binary byte for byte. |
| CWSDPMI 7a | [FreeDOS package catalog](https://ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/tools/cwsdpmi/20250320.0/index.html) | Local source snapshot `2025-03-18.2`; no Git commit recorded. The image executable matches the official `2025-03-20.0` package binary byte for byte. |
| UIDE 2020-07-07a | [FreeDOS package catalog](https://fd.lod.bz/repos/latest/html/en/drivers/uide/20250410.1/index.html) | Exact official package archive is `packages/uide.zip` (57,170 bytes, SHA-1 `b3dbdcc5118e19ad1474bba09768f33797186f58`, SHA-256 `56bfd95f288bab1cf361188974c9197ddb2441ad575632a9f7495597a4e986dd`). The image's `UIDE.SYS` matches the package binary byte for byte. |
| UDVD2 2022-12-12 | [FreeDOS package catalog](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/distributions/1.2/repos/html/en/drivers/udvd2/20221213.0/index.html) | Exact official package archive is `packages/udvd2.zip` (35,734 bytes, SHA-1 `c7aed9f82327aacb5d5adba8bf3a7efb21b623a0`, SHA-256 `de1890f21bf6622eb566a0b70e9776e9771861e61e55bf959fd5b79dc0850555`). The image's `UDVD2.SYS` matches the package binary byte for byte. |
| FINDRAMD.EXE in the image | Panthera source `sources/nfdos-findramd/FINDRAMD.C`, GPL-2.0-or-later | `third_party/nfdos-findramd-gpl/`, including `build-findramd-historical.ps1` |

The kernel and FreeCOM snapshots include the source edits in `patches/`. EDIT contains the five adapted source files listed in `PATCHES.md`. The exact component license and exceptions are summarized in `THIRD_PARTY_NOTICES.md` and preserved in the respective source directories.

The CABExtract, DOSFSCK, CWSDPMI, ATAPICDD, and SRDISK snapshots in `third_party/` contain source and notices only; compiled `BIN/` trees and packaged ZIP files were excluded. For the 2026-11-20 image refresh, the complete official UIDE and UDVD2 package ZIPs are included under `packages/` because those exact drivers are in the boot image. Each package ZIP retains its upstream files, including the official binary, source, documentation, and notices. The image driver's binary identity matches were checked byte for byte. This confirms provenance, not a reproducible build from the local source snapshots. Exact source/build correspondence for the remaining image binaries is still incomplete; see `DISTRIBUTION-COMPLIANCE.md`.

NF-DOS-authored startup files are under `src/boot/`. `FINDRAMD.C` and NFSPLASH sources are under `src/`. No NF-DOS image or project-built executable/object is part of the Git source tree. The release source archive separately carries the exact UIDE and UDVD2 upstream ZIPs under `packages/`. See `BUILDING.md` for the reproducibility boundary.

`src/findramd/` is a separate MIT-licensed Panthera copy. The floppy image's FINDRAMD.EXE was built from the GPL-2.0-or-later source in `third_party/nfdos-findramd-gpl/`; keep those grants separate.
