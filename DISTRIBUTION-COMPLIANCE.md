# NF-DOS 6.0 — distribuição e correspondência de fontes

Revisão técnica de avisos e proveniência, 2026-11-15. Isto não é parecer jurídico.

## Escopo auditado

A imagem candidata revisada é `NF-DOS-6.0/NF-DOS6.0-v21.img` (SHA-256 `EB387DA81E17D1F5D75BEC0C7CCD323EC5EBB82A5657C106306EA4BDF112FFBC`, 1.474.560 bytes). Ela contém avisos em `LICENSES/`, o texto integral de GPL-2 em `LICENSES/GPL-2.TXT` e `KERNEL/COPYING.TXT`, e `NFAPPS.ZIP`, que distribui executáveis e scripts de terceiros. A v21 foi inicializada no QEMU até `A:\>`; a captura PPM é byte-a-byte idêntica à captura validada da v20 e os logs estão em `NF-DOS-6.0/stability/NF-DOS5.02-vs-6.0-NFLLM-20261114/`.

`NFAPPS.ZIP` contém ATTRIB, CABEXT, CHKDSK.BAT, CTMOUSE, CWSDPMI, DBLSPACE.BAT, DEBUG, DOSFSCK, DRVSPACE.BAT, EDIT, EXT.BAT, EXTRACT.BAT, FDAPM, FORMAT, HELP.BAT, MSCDEX.BAT, NFHELP.TXT, RESTART.BAT, SCANDISK.BAT, SHSUCDX e SYS. A imagem também contém o kernel, FreeCOM, FDISK, FINDRAMD, ZIP/UNZIP, ATAPICDD, BITDISK, HIMEMX e NFSPLASH.

## Licenças e avisos

| Componente | Termos/atribuição | Fonte nesta publicação |
|---|---|---|
| FreeDOS Kernel 2046 | GPL-2.0-or-later; avisos upstream preservados | `third_party/freedos-kernel/`, com revisão em `SOURCES.md` |
| FreeCOM 0.87 | GPL-2.0; patch local também sob GPL | `third_party/freecom/` e `patches/freecom-nfdos.patch` |
| FDISK, ATTRIB, CHKDSK, EDIT, FORMAT, SYS, FDAPM | Termos por projeto; fontes e avisos upstream | diretórios correspondentes em `third_party/` |
| FINDRAMD.EXE da imagem | GPL-2.0-or-later, conforme o aviso e a cópia-fonte usada no build da imagem | `third_party/nfdos-findramd-gpl/`, incluindo `COPYING` e script de build registrado |
| DOSFSCK 2.11d | GPL-2.0 | `third_party/dosfsck/`; preserva metadados e avisos do pacote |
| CWSDPMI 7a | GPL-2.0 e aviso adicional para redistribuição do executável oficial inalterado | `third_party/cwsdpmi/`, incluindo `DOC/COPYING.CWS` |
| CABExtract 1.11+1 | GPL-3.0 | `third_party/cabext/` |
| CuteMouse | GPL-2.0 | `third_party/mouse/` |
| ZIP e UnZip | Licenças próprias Info-ZIP | `third_party/zip/`, `third_party/unzip/` e avisos integrais na imagem |
| SHSUCDX, DEBUG, BITDISK, ATAPICDD, HIMEMX | Avisos próprios/mistos; preservar textos e atribuições upstream | diretórios correspondentes em `third_party/` e `THIRD_PARTY_NOTICES.md` |
| NFSPLASH.COM | MIT para o código | `src/nfsplash/`; não licencia a arte |
| Logo original `def-splash.png` | Eu criei a arte. A Panthera Softwares é minha empresa e titular dos direitos; como proprietário, autorizo seu uso e redistribuição em nome da Panthera no NF-DOS 6.0 | autorização limitada em `LICENSE-SCOPE.md`; hash conferido no relatório de auditoria |

Os quatro snapshots CABExtract, DOSFSCK, CWSDPMI e ATAPICDD foram acrescentados a `third_party/` sem diretórios `BIN/` nem arquivos compilados. A fonte GPL do FINDRAMD usado pela imagem também foi publicada em `third_party/nfdos-findramd-gpl/`; `src/findramd/` é uma cópia MIT separada que eu, Busta, autorizo sob esse termo. A fonte do FreeCOM e seu patch permanecem identificados. O aviso MIT do NFSPLASH não se estende ao logo; a autorização do logo não se estende a outros ativos históricos.

## Correspondência de fontes e condição de distribuição

O repositório é source-only e não distribui a imagem nem programas compilados. Para uma distribuição futura da imagem ou de `NFAPPS.ZIP`, ainda é necessário associar cada executável ao conjunto exato de fontes, patches, arquivos de interface e instruções de build/instalação aplicáveis. A auditoria registrou diferenças de hash/tamanho entre os ZIPs locais de DOSFSCK/CWSDPMI e os metadados do catálogo FreeDOS; por isso, a identidade exata desses pacotes não está comprovada. A correspondência de outros binários oficiais pré-compilados também não deve ser presumida.

GPLv2 seção 3 prevê formas específicas para distribuir código-objeto, inclusive acompanhar o binário com o código-fonte correspondente completo ou usar uma das formas de oferta escrita descritas no texto. A mera presença de um repositório com fontes relacionadas não comprova, por si só, que o conjunto completo corresponda aos executáveis entregues. Até resolver a correspondência e a disponibilidade do conjunto completo, esta publicação **não declara a imagem liberada para redistribuição pública**. Consulte o texto integral em `LICENSES/GPL-2.TXT` e a [GPLv2 da FSF](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).

Também permanece pendente concluir a análise do runtime/saída produzido com Open Watcom e a condição aplicável da OWPL. A imagem inclui o texto OWPL, mas a presença de um aviso, sozinha, não resolve essa análise. O inventário de componentes e os avisos específicos estão em `THIRD_PARTY_NOTICES.md` e nos arquivos preservados em cada diretório.

## Avisos já verificados

- O texto integral da GPLv2 está incluído no disquete. Os avisos de GPLv3, Info-ZIP, MIT, permissões adicionais e outros componentes também permanecem separados por componente.
- O índice interno foi corrigido para apontar ao aviso MIT do código em `NFSPLASH.TXT`, na raiz da imagem, e agora identifica `FINDRAMD.EXE` como GPL-2.0-or-later com o aviso `FINDRAMD.TXT`. Os avisos da arte dizem em primeira pessoa que eu criei o logo, que a Panthera Softwares é minha empresa e titular dos direitos, e que autorizo seu uso em nome dela no NF-DOS 6.0. A autorização continua limitada à arte original.
- A imagem v21 inicializou no QEMU, mostrou `NF-DOS 6.0` e a linha de aniversário, e chegou ao prompt `A:\>` sem o rótulo `[FreeDOS Kernel 2046]`. A VM saiu normalmente (QEMU exit code 0; captura SHA-256 `80403E931C959E8FC8A8434E945651E2F21D2EFF4315B96AD9B94CE95630BCEB` idêntica à da v20; kernel, NFLLM e programas não foram alterados).
- Alterar a tela de inicialização para mostrar somente `NF-DOS 6.0` não altera licença, copyright nem atribuição. As identificações e avisos técnicos permanecem na documentação e nos arquivos de licença.

## Referências upstream

- [FreeDOS Kernel](https://github.com/FDOS/kernel) e [FreeCOM](https://github.com/FDOS/freecom)
- [CABExtract/libmspack](https://github.com/dajhorn/libmspack)
- [FreeDOS package catalog](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/)
- [Info-ZIP license](https://infozip.sourceforge.net/license.html)
- [Open Watcom license](https://github.com/open-watcom/open-watcom-v2/blob/master/license.txt)
