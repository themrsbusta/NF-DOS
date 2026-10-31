# Avisos de terceiros

A licença raiz MIT cobre somente o material Panthera listado em `LICENSE-SCOPE.md`. Os códigos e avisos de terceiros mantêm os termos próprios; não são relicenciados pela Panthera.

| Componente | Licença/aviso preservado | Observação |
|---|---|---|
| FreeDOS Kernel e country tables | GPL-2.0-or-later e avisos incluídos nas próprias fontes (`COPYING`, arquivos do subprojeto) | As alterações locais também permanecem sob os termos upstream. |
| FreeCOM | GPL-2.0, arquivo `license` e avisos auxiliares | O patch de renomeação de símbolo é alteração derivada e permanece GPL. |
| FDISK, ATTRIB, CHKDSK, EDIT, FORMAT e SYS | Licenças GPL e avisos por componente; confira `LICENSE`, `COPYING` ou `copying` nos respectivos diretórios | O CHKDSK contém subdiretórios com avisos adicionais; todos os arquivos de licença rastreados foram mantidos. |
| FDAPM | GPL-2.0-or-later, `disclaim.txt` | Fonte Assembly. |
| HIMEMX | As mudanças do projeto HIMEMX são declaradas domínio público; a base FDHimem mantém direitos/termos GPL e/ou Artistic, conforme `Readme.txt` | Aviso misto: não tratar todo o fonte como MIT ou domínio público. |
| BITDISK | O cabeçalho de `BITDISK.ASM` declara freeware/domínio público e preserva autoria/contribuições | O snapshot da árvore `FDOS/ramdisk` também contém outros componentes e avisos próprios; o Beta 2 usa BITDISK. |
| CuteMouse | GPL-2.0, `copying` | O Beta 2 usou um executável de pacote oficial; a correspondência binário-fonte exata não foi confirmada. Este repositório não inclui esse executável. |
| SHSUCDX | Licença permissiva customizada “as-is”, `LICENSE.txt` | Mantém atribuição, aviso e condições próprias. |
| DEBUG | Aviso permissivo customizado e contribuições em domínio público, conforme fontes/README | Clone de comando compatível; não é código-fonte Microsoft. |
| ZIP e UnZip | Licenças customizadas Info-ZIP, `LICENSE` e avisos associados | Preservadas junto às fontes. |

## Ferramentas de build e validação (não incluídas)

- [Open Watcom](https://github.com/open-watcom/open-watcom-v2) — Sybase Open Watcom Public License 1.0. Nenhum executável do toolchain é distribuído aqui; antes de publicar executáveis construídos com ele, revisar as obrigações da licença e do runtime.
- [NASM](https://github.com/netwide-assembler/nasm) — BSD-2-Clause.
- [JWasm](https://github.com/JWasm/JWasm) — consulte o `License.txt` da revisão usada, baseado na Sybase Open Watcom Public License 1.0.
- [QEMU](https://www.qemu.org/docs/master/about/license.html) — GPL-2.0, com partes sob avisos específicos; usado para validação e não distribuído aqui.

Esta tabela é um índice, não substitui os avisos integrais nos diretórios de origem. Em caso de diferença, prevalecem os arquivos de licença aplicáveis ao componente.
