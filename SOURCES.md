# Fontes e proveniência

Snapshot preparado para NF-DOS 6.0 Beta 2 v8, com base nas revisões upstream abaixo. Os diretórios preservam os arquivos de licença e avisos rastreados por cada projeto. Saídas compiladas e diretórios `.git` aninhados foram removidos.

Imagem de referência omitida: NF-DOS6.0-beta2-nfsplash-v8.img (SHA-256 876CB8CB40564B03A94E68F874EBB7B64C859303212E25782AFED7DD459E7B0D). O v8 deriva do v7 e contém a correção de saída do EDIT; o hash é metadado de proveniência, não um binário publicado.

| Componente | Repositório | Commit de origem |
|---|---|---|
| FreeDOS Kernel | [FDOS/kernel](https://github.com/FDOS/kernel) | `5ffb5502d39a10a30f5b8a9e8beeba0bf30245d3` |
| Tabelas de país | [FDOS/country](https://github.com/FDOS/country) | `7f83e041d00f78b3912c761246930f3b437440f6` |
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

The kernel and FreeCOM snapshots include the source edits in `patches/`. EDIT contains the five adapted source files used for v8. The exact component license and exceptions are summarized in `THIRD_PARTY_NOTICES.md` and preserved in the respective source directories.

NF-DOS-authored startup files are under `src/boot/`. `FINDRAMD.C` and NFSPLASH sources are under `src/`. No image, executable, driver, object file, archive, or compiler output is part of this tree. See `BUILDING.md` for the reproducibility boundary.
