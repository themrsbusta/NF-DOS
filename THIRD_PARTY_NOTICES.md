# Third-party notices

The root MIT license covers only the Panthera material listed in `LICENSE-SCOPE.md`. Third-party code and notices retain their own terms; they are not relicensed by Panthera.

| Component | Preserved license/notice | Notes |
|---|---|---|
| FreeDOS Kernel and country tables | GPL-2.0-or-later and notices included with the sources (`COPYING`, subproject files) | Local changes also remain under the upstream terms. |
| FreeCOM | GPL-2.0, `license` file, and auxiliary notices | The symbol-renaming patch is a derivative change and remains GPL. |
| FDISK, ATTRIB, CHKDSK, EDIT, FORMAT, and SYS | GPL licenses and component notices; see `LICENSE`, `COPYING`, or `copying` in the respective directories | CHKDSK contains subdirectories with additional notices; all tracked license files were retained. |
| FDAPM | GPL-2.0-or-later, `disclaim.txt` | Assembly source. |
| HIMEMX | HIMEMX project changes are declared public domain; the FDHimem base retains GPL and/or Artistic rights/terms, as stated in `Readme.txt` | Mixed notice: do not treat all source as MIT or public domain. |
| BITDISK | `BITDISK.ASM` header declares freeware/public domain and preserves authorship/contributions | The `FDOS/ramdisk` tree snapshot also contains other components and their own notices; Beta 2 uses BITDISK. |
| CuteMouse | GPL-2.0, `copying` | Beta 2 used an executable from an official package; the exact source-to-binary match was not confirmed. This repository does not include that executable. |
| SHSUCDX | Custom permissive “as-is” license, `LICENSE.txt` | Its own attribution, notice, and conditions remain in effect. |
| DEBUG | Custom permissive notice and public-domain contributions, as stated in the source/README | Compatible command clone; not Microsoft source code. |
| ZIP and UnZip | Custom Info-ZIP licenses, `LICENSE`, and associated notices | Preserved alongside the source. |

## Build and validation tools (not included)

- [Open Watcom](https://github.com/open-watcom/open-watcom-v2) — Sybase Open Watcom Public License 1.0. No toolchain executable is distributed here; review the license and runtime obligations before publishing executables built with it.
- [NASM](https://github.com/netwide-assembler/nasm) — BSD-2-Clause.
- [JWasm](https://github.com/JWasm/JWasm) — see the `License.txt` for the revision used, based on the Sybase Open Watcom Public License 1.0.
- [QEMU](https://www.qemu.org/docs/master/about/license.html) — GPL-2.0, with some parts under specific notices; used for validation and not distributed here.

This table is an index and does not replace the full notices in the source directories. If anything differs, the license files applicable to the component take precedence.
