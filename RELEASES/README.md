# NF-DOS 6.0 release assets

Download the ISO, floppy image, historical source archive and checksums from the [v6.0 release](https://github.com/themrsbusta/NF-DOS/releases/tag/v6.0).

The branch contains the browsable core source tree. The release assets remain separate. To verify the original release package using its original checksum manifest, download its original source ZIP to a local `SOURCE/NF-DOS-6.0-source.zip` path and both disk images to `RELEASES/`. From the repository root, run:

```sh
sha256sum -c DOCS/SHA256SUMS.txt
```

`DOCS/NF-DOS_60_Release.yaml` and `DOCS/SHA256SUMS.txt` are historical release records, not a manifest of the current source tree. The v6.0 tag and its assets have not been changed.
