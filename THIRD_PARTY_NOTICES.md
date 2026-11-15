# Third-party notices

The root MIT license covers only the Panthera material listed in `LICENSE-SCOPE.md`. I created the original logo. Panthera Softwares is my company and holds the artwork rights; as its owner, I authorize use and redistribution of the logo on Panthera's behalf as part of NF-DOS 6.0. This authorization is separate from the MIT license for NFSPLASH code. Third-party code and notices retain their own terms; they are not relicensed by Panthera.

| Component | Preserved license/notice | Notes |
|---|---|---|
| FreeDOS Kernel and country tables | GPL-2.0-or-later and notices included with the sources (`COPYING`, subproject files) | Local changes also remain under the upstream terms. |
| FreeCOM | GPL-2.0, `license` file, and auxiliary notices | The symbol-renaming patch is a derivative change and remains GPL. |
| FINDRAMD.EXE in the image | GPL-2.0-or-later, Panthera source used for the image build | Source, GPL text, and build script in `third_party/nfdos-findramd-gpl/`; the separate `src/findramd/` copy is MIT. |
| FINDRAMD in `src/findramd/` | MIT, separate Panthera grant | This source is not the tree used for the image executable. |
| FDISK, ATTRIB, CHKDSK, EDIT, FORMAT, and SYS | GPL licenses and component notices; see `LICENSE`, `COPYING`, or `copying` in the respective directories | CHKDSK contains subdirectories with additional notices; all tracked license files were retained. |
| FDAPM | GPL-2.0-or-later, `disclaim.txt` | Assembly source. |
| CABExtract 1.11+1 | GPL-3.0, `LICENSE` and package metadata | Source in `third_party/cabext/`; it reads CAB format but does not include Microsoft code. |
| DOSFSCK 2.11d | GPL-2.0, package notices and full GPL text | Source in `third_party/dosfsck/`; exact correspondence to the packaged executable remains unresolved. |
| CWSDPMI 7a | GPL-2.0 and upstream exception for redistributing the unmodified official binary | Source and `DOC/COPYING.CWS` in `third_party/cwsdpmi/`; exact archive/binary identity remains unresolved. |
| HIMEMX | HIMEMX project changes are declared public domain; the FDHimem base retains GPL and/or Artistic rights/terms, as stated in `Readme.txt` | Mixed notice: do not treat all source as MIT or public domain. |
| BITDISK | `BITDISK.ASM` header declares freeware/public domain and preserves authorship/contributions | The `FDOS/ramdisk` tree snapshot also contains other components and their own notices; the NF-DOS 6.0 image uses BITDISK. |
| ATAPICDD 0.2.14 | Author's public-domain declaration, with notice and warranty disclaimer preserved | Source in `third_party/atapicdd/`; attribution to Kenneth J. Davis and the ATADRVR base is retained. |
| CuteMouse | GPL-2.0, `copying` | Source in `third_party/mouse/`; exact correspondence to the included executable remains unconfirmed. |
| SHSUCDX | Custom permissive “as-is” license, `LICENSE.txt` | Its own attribution, notice, and conditions remain in effect. |
| DEBUG | Custom permissive notice and public-domain contributions, as stated in the source/README | Compatible command clone; not Microsoft source code. |
| ZIP and UnZip | Custom Info-ZIP licenses, `LICENSE`, and associated notices | Preserved alongside the source. |

## Build and validation tools (not included)

- [Open Watcom](https://github.com/open-watcom/open-watcom-v2) — Sybase Open Watcom Public License 1.0. No toolchain executable is distributed here; review the license and runtime obligations before publishing executables built with it.
- [NASM](https://github.com/netwide-assembler/nasm) — BSD-2-Clause.
- [JWasm](https://github.com/JWasm/JWasm) — see the `License.txt` for the revision used, based on the Sybase Open Watcom Public License 1.0.
- [QEMU](https://www.qemu.org/docs/master/about/license.html) — GPL-2.0, with some parts under specific notices; used for validation and not distributed here.

This table is an index and does not replace the full notices in the source directories. If anything differs, the license files applicable to the component take precedence.

## Floppy image and corresponding source

The NF-DOS 6.0 floppy contains `NFAPPS.ZIP` with executables from several components listed above. Identified sources are published without executables, but exact correspondence between every distributed binary and its source, patches, and build/installation instructions has not been completed. See `DISTRIBUTION-COMPLIANCE.md`; the presence of a license file or a similar source version does not prove full correspondence or clear the image for public binary redistribution.
