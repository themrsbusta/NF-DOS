# NF-DOS 6.0

NF-DOS was my first project of my own. I started it in 2006, using the Windows 98 SE startup disk as a base. In 2026, I resumed it to celebrate its 20th anniversary, preserve the NF-DOS identity, and replace its core with FreeDOS technology. The product is NF-DOS 6.0.

This publication contains **source code, documentation, license notices, and the original artwork used as input for the splash screen only**. It does not contain a floppy disk image or compiled programs (`.COM`, `.EXE`, `.SYS`, `.IMG`, or equivalent files). I created the original NF-DOS logo. Panthera Softwares is my company and holds the artwork rights; as its owner, I authorize use and redistribution of the logo on Panthera's behalf as part of NF-DOS 6.0. See [LICENSE-SCOPE.md](LICENSE-SCOPE.md) for the limited scope of that authorization.

## Contents

- FreeDOS Kernel 2046 and FreeCOM source, with the NF-DOS 6.0 changes.
- Source for FreeDOS and third-party utilities listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), each under its own terms.
- Panthera code for `NFSPLASH.COM` and `FINDRAMD`, startup/installation scripts, and the source logo `def-splash.png`.
- Upstream revisions, local changes, and build limitations documented in [SOURCES.md](SOURCES.md), [PATCHES.md](PATCHES.md), and [BUILDING.md](BUILDING.md).

The kernel used is FreeDOS Kernel 2046 (v2.46), identified upstream as the June 29, 2026 release: [official release](https://github.com/FDOS/kernel/releases/tag/ke2046). The shell is FreeCOM 0.87.

## Publication limits

This is a source package for the identified NF-DOS components and scripts. **It is not yet a fully reproducible, end-to-end recipe for recreating the NF-DOS 6.0 floppy byte for byte.** The reference image, historical boot sector, and integration binaries were deliberately omitted. The repository also does not claim that the source origin of every legacy file in the image has been identified.

The goal of this publication is to keep the identified source and its notices available without distributing binaries. See [BUILDING.md](BUILDING.md) for limitations and dependencies.

## Licenses

Original Panthera files in this repository are provided under MIT within the scope defined in [LICENSE-SCOPE.md](LICENSE-SCOPE.md). **MIT does not replace or relicense third-party code.** External components remain subject to the notices and licenses preserved in each `third_party/` directory and described in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Directories

- `src/boot/` — NF-DOS startup scripts and configuration.
- `src/findramd/`, `src/nfsplash/` — original Panthera code.
- `third_party/` — upstream source snapshots, without nested Git history or compiled output.
- `patches/` — local changes to third-party components; they remain under the applicable upstream licenses.

Contributions and fixes are welcome. Before adding components, preserve the original license and record the source revision.
