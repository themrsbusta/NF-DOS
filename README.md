# NF-DOS 6.0

**Division:** Panthera Softwares. **License:** [MIT](LICENSE) (original Panthera material only).


[![CD Edition - Panthera Works](https://img.shields.io/badge/CD_Edition-Panthera_Works-2563eb?style=flat-square)](https://github.com/themrsbusta/Panthera-Works) [![CD Edition - NFLLM](https://img.shields.io/badge/CD_Edition-NFLLM-2563eb?style=flat-square)](https://github.com/themrsbusta/NFLLM)

*Included in the CD Edition.*

NF-DOS (New Frontiers DOS) began in 2006 and was revived as NF-DOS 6.0. It uses FreeDOS Kernel 2046 and FreeCOM 0.87, with Panthera utilities, startup scripts and a CD edition containing additional applications.

Developed by Panthera Softwares, a division of Panthera Research.

## Download NF-DOS 6.0

- [Bootable CD ISO](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS6.0.iso)
- [1.44 MB floppy image](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS6.0.img)
- [Source archive](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/NF-DOS-6.0-source.zip)
- [Release checksums](https://github.com/themrsbusta/NF-DOS/releases/download/v6.0/SHA256SUMS.txt)
- [Release v6.0](https://github.com/themrsbusta/NF-DOS/releases/tag/v6.0)

## Release asset refresh — 2026-11-20

The v6.0 release assets were refreshed under the same public filenames. The floppy and ISO use UIDE and UDVD2 drivers; the source archive includes the exact official FreeDOS packages with their source, documentation and notices. The bundled Panthera Works directory and product headings use **Panthera Works 1.0**. QEMU 8.2.2 reached the NF-DOS 6.0 screen and `A:\>` prompt from both images. Current hashes are in `DOCS/SHA256SUMS.txt`.

## Source code

- `src/` — NF-DOS startup configuration, FINDRAMD and NFSPLASH.
- `third_party/` — identified kernel, shell, tools and upstream source snapshots.
- `patches/` — local modifications to upstream components.
- `DOCS/` — release manifest and checksums.

See [BUILDING.md](BUILDING.md), [SOURCES.md](SOURCES.md) and [PATCHES.md](PATCHES.md) for build information.

The identified core sources are available directly in this branch. The source archive accompanying v6.0 is not a complete build kit for every program on the CD. Missing source/build correspondence and distribution review items are recorded in [DISTRIBUTION-COMPLIANCE.md](DISTRIBUTION-COMPLIANCE.md).

## License

Panthera Research licenses its original material under MIT as specified in [LICENSE-SCOPE.md](LICENSE-SCOPE.md). Third-party components retain their original licenses and credits; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). The v6.0 tag name and public asset filenames remain the same; the asset contents and hashes were updated on 2026-11-20.
