# Release assets

The final public product name is **NF-DOS 6.0**. Download `NF-DOS6.0.img`, `NF-DOS6.0.iso`, and `NF-DOS-6.0-source.zip` from the [NF-DOS 6.0 GitHub Release](https://github.com/themrsbusta/NF-DOS/releases/tag/v6.0).

The disk images are intentionally kept as Release assets because the ISO is larger than GitHub's per-file Git limit. To verify the complete handoff, place the two image files here as `NF-DOS6.0.img` and `NF-DOS6.0.iso`, then run `sha256sum -c DOCS/SHA256SUMS.txt` from the repository root.
