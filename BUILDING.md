# Building

## Source and tools

This repository does not include compilers, assemblers, compiled DOS programs, or disk images. Upstream source trees retain their own build files and instructions. The NFSPLASH build is documented in `src/nfsplash/README.md` and requires Python 3 and NASM.

The other components were built in the original environment with tools such as Open Watcom, NASM, and JWasm, following the respective projects' instructions. Install compatible versions and follow the upstream documentation included in each directory. These tools are not part of this publication; their references and licenses are listed in `THIRD_PARTY_NOTICES.md`.

## Rebuilding the floppy image

The complete Beta 2 image **cannot be reproduced from this repository alone**. The historical build chain depends on a base image and a binary boot sector that are not included, as well as compiled inputs whose exact source-to-binary correspondence has not been fully verified. These artifacts were omitted to keep this publication source-only and avoid redistributing legacy material without complete provenance.

Therefore, do not use this tree as proof that the `.IMG` image was built entirely from the included sources. This release publishes the identified source, patches, NF-DOS scripts, and licensing notices with explicit provenance.
