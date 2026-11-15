# Local changes

The changes below were developed during the historical Beta 2 work and are part of NF-DOS 6.0. They remain under the upstream components' licenses. Textual patches are alongside this file.

- **FreeDOS Kernel 2046** (`patches/freedos-kernel-nfdos.patch`): silent boot-banner configuration, removal of the F5/F8 pause, and suppression of the no-hard-disk message.
- **FreeCOM 0.87** (`patches/freecom-nfdos.patch`): renames `toUpper` to `supplToUpper` at collision points. This avoids unintended recursion and the memory corruption that appeared as `E_NoMem`.
- **EDIT**: the five source files used for Beta 2 v8 are in `third_party/edit/source/`: `console.c`, `edit.c`, `menubar.c`, `message.c`, and `video.c`. They include build-compatibility adjustments and a correct return to DOS with Alt+X/Alt+F4. The resulting executable is not included.

The EDIT sources start from the commit recorded in `SOURCES.md`. The compiled patch was tested with the v8 diagnostic image; that result does not make this source publication a fully reproducible build.
