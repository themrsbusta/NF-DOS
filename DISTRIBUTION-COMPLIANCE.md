# NF-DOS 6.0 — distribuição e correspondência de fontes

Revisão técnica de avisos e proveniência, 2026-11-15. Isto não é parecer jurídico.

## Escopo auditado

Uma imagem local da edição Floppy do NF-DOS 6.0 foi revisada quanto aos avisos e à correspondência das fontes. A imagem não está incluída neste repositório source-only. Ela preserva os avisos em `LICENSES/`, o texto integral de GPL-2 em `LICENSES/GPL-2.TXT` e `KERNEL/COPYING.TXT`, e `NFAPPS.ZIP`, que contém executáveis e scripts de terceiros.

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
- A imagem de avaliação do NF-DOS 6.0 inicializou no QEMU, mostrou `NF-DOS 6.0` e a linha de aniversário, e chegou ao prompt `A:\>` sem o rótulo `[FreeDOS Kernel 2046]`. `HELP` mostrou a ajuda atualizada, `EDIT` abriu e fechou normalmente, e a matriz NFLLM terminou com 31/31 casos aprovados, sem falhas; QEMU saiu com código 0. As evidências estão em `NF-DOS-6.0/stability/NF-DOS5.02-vs-6.0-NFLLM-20261114/`.
- Alterar a tela de inicialização para mostrar somente `NF-DOS 6.0` não altera licença, copyright nem atribuição. As identificações e avisos técnicos permanecem na documentação e nos arquivos de licença.

## Referências upstream

- [FreeDOS Kernel](https://github.com/FDOS/kernel) e [FreeCOM](https://github.com/FDOS/freecom)
- [CABExtract/libmspack](https://github.com/dajhorn/libmspack)
- [FreeDOS package catalog](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/)
- [Info-ZIP license](https://infozip.sourceforge.net/license.html)
- [Open Watcom license](https://github.com/open-watcom/open-watcom-v2/blob/master/license.txt)

## Reavaliação final da distribuição binária — 2026-11-15

A auditoria paralela da Mica foi concluída para a edição Floppy do NF-DOS 6.0. A conclusão permanece **UNRESOLVED** para a distribuição binária: a correspondência exata entre fonte, patches, instruções de build e cada um dos 21 executáveis inventariados ainda não está demonstrada. O resultado técnico de instalação e boot no QEMU não resolve essa pendência documental.

Os avisos revisados estão presentes na imagem, mas isso, por si só, não comprova a proveniência de cada binário. A identidade do pacote DOSFSCK e do executável CWSDPMI continua sem correspondência confirmada com os registros locais; a origem exata do CuteMouse e o efeito do runtime/saída do Open Watcom sob a OWPL também permanecem **UNRESOLVED**. A documentação oficial do catálogo FreeDOS identifica DOSFSCK 2.11d e CWSDPMI 7a como GPL-2, com hashes de pacote publicados, mas os hashes locais registrados não correspondem a esses registros. [DOSFSCK](https://ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/disk/dosfsck/20250409.8/index.html), [CWSDPMI](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/1.4/html/en/tools/cwsdpmi/20250318.2/index.html), [licença Open Watcom](https://github.com/open-watcom/open-watcom-v2/blob/master/license.txt).

**Recomendação atual: BLOQUEADO para distribuir a imagem binária.** Não há evidência suficiente para classificar esses itens como não conformes, mas também não há base para declarar a redistribuição liberada. Este relatório é uma verificação documental de engenharia, não um parecer jurídico. A futura CD Edition não foi tratada como artefato distribuível e precisará de auditoria própria quando houver um manifesto.

Na validação técnica limitada concluída nesta data, a instalação em destino descartável de 500 MiB, o boot do C: sem disquete, o prompt, DIR/VER, NFTEST SELFTEST e uma inferência NFLLM de um token passaram. A imagem candidata não mudou. Permanecem os limites operacionais descritos acima, incluindo FORMAT no BITDISK RAMDisk e cobertura apenas parcial dos utilitários. Evidências e o relatório YAML estão em `NF-DOS-6.0/stability/NF-DOS5.02-vs-6.0-NFLLM-20261114/`.

O nome e a versão do produto são **NF-DOS 6.0** / **6.0**. Este repositório contém fontes e documentação; não inclui imagem de disquete nem programas compilados. O envio ao repositório contém somente fontes e documentação; não inclui imagem, binários, tag ou Release.
