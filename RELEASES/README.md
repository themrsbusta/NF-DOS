# NF-DOS 6.0 release assets

Download the ISO, floppy image, source archive, compliance report and checksums from the [v6.0 release](https://github.com/themrsbusta/NF-DOS/releases/tag/v6.0).

The release assets were refreshed on 2026-11-20 while retaining the public tag and filenames. To verify the current package, download the source ZIP to `SOURCE/NF-DOS-6.0-source.zip` and both images to `RELEASES/`. From the repository root, run:

```sh
sha256sum -c DOCS/SHA256SUMS.txt
```

`DOCS/NF-DOS_60_Release.yaml` and `DOCS/SHA256SUMS.txt` document the current release assets and their updated SHA-256 checksums. The browsable core source tree remains in this branch; release images and the source archive remain separate assets.
