# NF-DOS 6.0 — distribuição e correspondência de fontes

Revisão técnica de avisos e proveniência, 2026-11-15. Isto não é parecer jurídico.

## Escopo auditado

Uma imagem local de disquete do NF-DOS 6.0 foi revisada quanto aos avisos e à correspondência das fontes. Este arquivo de fontes não inclui a imagem. Ela preserva os avisos em `LICENSES/`, o texto integral de GPL-2 em `LICENSES/GPL-2.TXT` e `KERNEL/COPYING.TXT`, e `NFAPPS.ZIP`, que contém executáveis e scripts de terceiros.

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
| Logo original `def-splash.png` | Eu criei a arte. A Panthera Research é titular dos direitos; autorizo seu uso e redistribuição no NF-DOS 6.0 | autorização limitada em `LICENSE-SCOPE.md`; hash conferido no relatório de auditoria |

Os snapshots CABExtract, DOSFSCK, CWSDPMI, ATAPICDD e SRDISK foram acrescentados a `third_party/` sem diretórios `BIN/` nem arquivos compilados. A fonte GPL do FINDRAMD usado pela imagem também foi publicada em `third_party/nfdos-findramd-gpl/`; `src/findramd/` é uma cópia MIT separada que eu, Busta, autorizo sob esse termo. A fonte do FreeCOM e seu patch permanecem identificados. O aviso MIT do NFSPLASH não se estende ao logo; a autorização do logo não se estende a outros ativos históricos.

## Correspondência de fontes e condição de distribuição

Este arquivo de fontes não contém a imagem completa do NF-DOS nem os aplicativos compilados do projeto; esses ativos são distribuídos separadamente no Release público. Ele inclui os binários upstream UIDE.SYS e UDVD2.SYS somente dentro dos pacotes oficiais completos, junto com suas fontes, documentação e avisos. Uma verificação completa da imagem requer associar cada executável ao conjunto exato de fontes, patches, arquivos de interface e instruções de build/instalação aplicáveis. A presença de fontes relacionadas não prova, por si só, que elas correspondam ao executável entregue.

GPLv2 seção 3 prevê formas específicas para distribuir código-objeto, inclusive acompanhar o binário com o código-fonte correspondente completo ou usar uma das formas de oferta escrita descritas no texto. A mera presença de um repositório com fontes relacionadas não comprova, por si só, que o conjunto completo corresponda aos executáveis entregues. Até resolver a correspondência e a disponibilidade do conjunto completo, esta publicação **não declara a imagem liberada para redistribuição pública**. Consulte o texto integral em `LICENSES/GPL-2.TXT` e a [GPLv2 da FSF](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).

Também permanece pendente concluir a análise do runtime/saída produzido com Open Watcom e a condição aplicável da OWPL. A imagem inclui o texto OWPL, mas a presença de um aviso, sozinha, não resolve essa análise. O inventário de componentes e os avisos específicos estão em `THIRD_PARTY_NOTICES.md` e nos arquivos preservados em cada diretório.

## Avisos já verificados

- O texto integral da GPLv2 está incluído no disquete. Os avisos de GPLv3, Info-ZIP, MIT, permissões adicionais e outros componentes também permanecem separados por componente.
- O índice interno foi corrigido para apontar ao aviso MIT do código em `NFSPLASH.TXT`, na raiz da imagem, e agora identifica `FINDRAMD.EXE` como GPL-2.0-or-later com o aviso `FINDRAMD.TXT`. Os avisos da arte dizem em primeira pessoa que eu criei o logo, que a Panthera Research detém os direitos e que autorizo seu uso no NF-DOS 6.0. A autorização continua limitada à arte original.
- A imagem de avaliação do NF-DOS 6.0 inicializou no QEMU, mostrou `NF-DOS 6.0` e a linha de aniversário, e chegou ao prompt `A:\>` sem o rótulo `[FreeDOS Kernel 2046]`. `HELP` mostrou a ajuda atualizada, `EDIT` abriu e fechou normalmente, e a matriz NFLLM terminou com 31/31 casos aprovados, sem falhas; QEMU saiu com código 0. As evidências estão em `NF-DOS-6.0/stability/NF-DOS5.02-vs-6.0-NFLLM-20261114/`.
- Alterar a tela de inicialização para mostrar somente `NF-DOS 6.0` não altera licença, copyright nem atribuição. As identificações e avisos técnicos permanecem na documentação e nos arquivos de licença.

## Referências upstream

- [FreeDOS Kernel](https://github.com/FDOS/kernel) e [FreeCOM](https://github.com/FDOS/freecom)
- [CABExtract/libmspack](https://github.com/dajhorn/libmspack)
- [FreeDOS package catalog](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/)
- [Info-ZIP license](https://infozip.sourceforge.net/license.html)
- [Open Watcom license](https://github.com/open-watcom/open-watcom-v2/blob/master/license.txt)

## Reavaliação da distribuição binária — 2026-11-16

Uma revisão secundária da documentação foi feita com apoio da Mica; o cruzamento dos binários da imagem com os pacotes oficiais foi verificado separadamente. A revisão da Mica é apoio técnico, não auditoria jurídica independente.

### Identidade de pacotes confirmada

- `DOSFSCK.EXE` (2.11d) corresponde byte a byte a `BIN/DOSFSCK.EXE` do pacote FreeDOS `2025-04-09.8`. O SHA-256 do executável da imagem é `49655E881E23F41CF66533C00ACAD2FC745493624CF45160D2B2AE11CC03925C`. O pacote oficial tem 141.641 bytes, SHA-1 `E97E142108C887001C31C55AF60468F6B602C06D` e licença GPL-2. A identidade do binário está confirmada; uma reconstrução independente e reproduzível ainda não foi demonstrada. [Catálogo oficial DOSFSCK](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/disk/dosfsck/20250409.8/index.html).
- `CWSDPMI.EXE` (7a) corresponde byte a byte a `BIN/CWSDPMI.EXE` do pacote FreeDOS `2025-03-20.0`. O SHA-256 do executável da imagem é `2DE899FECAA90632B8B9BDFC0305CB0375E59AE252C37E32D06C1ED3F98A8F44`. O pacote oficial tem 163.308 bytes, SHA-1 `275AC4545058E829E6D656CDE529817ABA77C890`, GPL-2 e inclui `DOC/COPYING.CWS`, com autorização adicional para redistribuição do binário oficial inalterado sob suas condições. A identidade do binário está confirmada; uma reconstrução independente e reproduzível ainda não foi demonstrada. [Catálogo oficial CWSDPMI](https://ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/latest/html/en/tools/cwsdpmi/20250320.0/index.html).
- `SRDISK.EXE` e `SRDXMS.SYS` correspondem byte a byte aos binários do pacote FreeDOS 1.4 de SRDISK 2.09d, GPL-2. O SHA-256 dos executáveis da imagem é, respectivamente, `D46837298325E92BCA311FCACB3F55891DFB2E5903F71299A0B352BB4D444A7A` e `59D4539846E37C56D4C4358E39C73B254BB4641EA893E68E77C96DB498A654AF`. O pacote oficial tem 226.776 bytes e SHA-256 `CCC8DE3F3D544D7D1C3DB394D82A435844C9A42BF620CF44475EA1F4D8A8630C`; o código-fonte, os avisos e as licenças correspondem entre os pacotes, exceto metadados `APPINFO/SRDISK.LSM`. A comparação confirma proveniência, não compilação independente. [Pacote FreeDOS SRDISK](https://www.ibiblio.org/pub/micro/pc-stuff/freedos/files/repositories/1.4/drivers/srdisk.zip).

### Inventário dos binários da imagem

| Arquivo(s) | Licença/aviso preservado | Situação da associação fonte-binário |
|---|---|---|
| `KERNEL.SYS` | FreeDOS GPL-2.0-or-later | Fontes do kernel e patches estão publicados; a imagem não é distribuída neste repositório. |
| `COMMAND.COM` | FreeCOM GPL-2.0 | Fonte e patch estão publicados; o binário não é distribuído aqui. |
| `FINDRAMD.EXE` | GPL-2.0-or-later, conforme o aviso e a fonte Panthera usada | Fonte GPL e instruções de build estão publicadas. |
| `FORMAT.EXE` | FreeDOS GPL-2.0; compilado com Open Watcom | Patch-fonte e build limpo estão registrados; obrigações OWPL aplicáveis ao resultado ainda precisam de revisão antes de redistribuir o executável. |
| `DOSFSCK.EXE` | GPL-2 | Correspondência exata com pacote oficial confirmada acima; build independente não verificado. |
| `CWSDPMI.EXE` | GPL-2 e permissão adicional `COPYING.CWS` | Correspondência exata com pacote oficial confirmada acima; build independente não verificado. |
| `SRDISK.EXE`, `SRDXMS.SYS` | GPL-2 | Correspondência exata com pacote oficial do FreeDOS confirmada; build independente não verificado. |
| `ATTRIB.COM`, `FDISK.EXE`, `EDIT.EXE`, `SYS.COM`, `FDAPM.COM` | Licenças FreeDOS e avisos por componente preservados | Fontes estão publicadas; a correspondência exata de cada executável e sua receita de build não está toda comprovada. |
| `CABEXT.EXE` | GPL-3.0 | Fonte está publicada; correspondência exata e build reproduzível não verificados. |
| `CTMOUSE.EXE` | GPL-2.0 | Fonte e licença estão publicadas; correspondência exata e build reproduzível não verificados. |
| `DEBUG.COM` | Aviso permissivo e contribuições em domínio público, conforme os arquivos upstream | Fonte está publicada; correspondência exata não verificada. |
| `SHSUCDX.COM` | Licença permissiva própria “as-is” | Fonte e licença estão publicadas; correspondência exata não verificada. |
| `ZIP.EXE`, `UNZIP.EXE` | Licenças próprias Info-ZIP | Fontes e avisos estão publicados; correspondência exata não verificada. |
| `ATAPICDD.SYS` | Declaração de domínio público do autor, com avisos preservados | Fonte e avisos estão publicados; correspondência exata não verificada. |
| `BITDISK.EXE` (inativo) | Aviso freeware/domínio público da fonte | Componente não ativo; fonte está publicada; correspondência exata não verificada. |
| `HIMEMX.EXE` | Termos mistos do HIMEMX/FDHimem: GPL, Artistic e domínio público conforme cada parte | Fonte e avisos estão publicados; correspondência exata não verificada. |
| `NFSPLASH.COM` | MIT para o código; a arte original tem autorização separada | Fonte está publicada. A MIT não licencia a arte. |

A imagem contém **23 arquivos executáveis ou drivers** nessa auditoria: 11 em `NFAPPS.ZIP` e 12 na raiz/sistema. Os scripts BAT e textos incluídos no arquivo foram inventariados, mas não contados como binários. A matriz acima mantém separadas a licença declarada, a identidade do pacote e a prova de compilação; são verificações diferentes.

### Decisão de distribuição

- **Arquivo de fontes identificado: APTO para distribuição como fontes e documentação.** Este ZIP contém código-fonte, avisos e arte autorizada, sem a imagem completa do NF-DOS nem os aplicativos compilados do projeto. Ele também contém os pacotes oficiais completos UIDE/UDVD2, que incluem os respectivos binários upstream junto com fonte, documentação e avisos.
- **Revisão técnica da redistribuição binária: INCOMPLETA.** A revisão não conseguiu confirmar a correspondência fonte, patch e build de todos os executáveis, nem concluir a avaliação das obrigações Open Watcom Public License 1.0 aplicáveis ao `FORMAT.EXE` e aos componentes runtime. Os arquivos OWPL exigem preservar avisos e cumprir condições de disponibilização e licença do código coberto; a documentação oficial da [OWPL 1.0](https://open-watcom.github.io/open-watcom-v2-wikidocs/c_readme.html) define essas condições. Não se concluiu que haja infração, mas também não há base para afirmar que a revisão da imagem completa esteja concluída.

Este relatório é uma verificação documental de engenharia, não um parecer jurídico. A imagem ISO deste lançamento não foi auditada. O nome e a versão publicados são **NF-DOS 6.0** / **6.0**; nenhum rótulo de candidata interna é usado neste repositório.

## Regressão técnica

### Execução final — 2026-11-16

- A imagem de disquete de 1.474.560 bytes foi validada pelo SHA-256 `53C7345445F70B71D93F2094CAE406BF5E79E4B1AE042C94551B6DE6C9B18C62`. A regressão integrada usou o disco de instalação de 500 MB, com o hash-base conferido antes e depois.
- A suíte registrou 77 aprovações e uma reprovação automatizada. A reprovação foi `CALL batch fixture`: a captura VGA mostra a chamada de `NF60BAT.BAT`, o comando escrito pelo BAT, a saída `nf-dos-bat-ok` e o retorno ao prompt. O padrão do verificador exigia letras maiúsculas e era sensível a maiúsculas/minúsculas; a evidência confirma que o BAT executou. Uma execução anterior do mesmo fixture também registrou PASS. Classificação: **falso negativo do harness**, sem defeito reproduzido no COMMAND.COM.
- Os 31 casos da matriz NFLLM passaram, cobrindo os modelos e os caminhos de backend/ISA incluídos no conjunto de teste. Os arquivos do NFLLM e do llama.cpp não foram alterados.
- `EDIT` abriu, teve a tela capturada, fechou com Alt-X e retornou ao prompt DOS.
- QEMU foi encerrado ao final; não restaram processos QEMU gerenciados ativos.

**Resultado técnico:** não foi reproduzido defeito funcional nesta regressão. O registro bruto continua mostrando 77 aprovações e uma reprovação do harness; sua causa foi confirmada pela saída VGA. O processo supervisor terminou com código 1 por causa dessa marcação automatizada.

## Estado de publicação — 2026-11-20

O proprietário orientou a publicação pública do NF-DOS 6.0. O GitHub Release `v6.0` distribui a imagem `.img`, a ISO inicializável e o arquivo ZIP de fontes. A publicação segue identificada exclusivamente como **NF-DOS 6.0**.

Essa decisão de publicação não altera as conclusões técnicas acima nem declara a redistribuição integralmente liberada. A auditoria detalhada cobre a imagem de disquete; a ISO incluída no lançamento não foi auditada. Permanecem sem resolução a correspondência completa entre alguns binários e suas fontes/instruções, a revisão das obrigações Open Watcom e os direitos de redistribuição dos modelos TinyStories e MCUNet listados no pacote de lançamento. Não há infração comprovada; há itens pendentes que impedem afirmar que a revisão técnica/jurídica está concluída.


## Atualização dos ativos do Release — 2026-11-20

Os ativos `NF-DOS6.0.img` e `NF-DOS6.0.iso` foram atualizados a partir dos arquivos entregues na pasta `update`. Os drivers `UIDE.SYS` e `UDVD2.SYS` da imagem correspondem byte a byte aos binários dos pacotes oficiais FreeDOS UIDE `2020-07-07a` e UDVD2 `2022-12-12`. A imagem ISO contém o mesmo disquete atualizado como `NF-DOS 6.0 Boot/BOOT.IMG`.

| Arquivo | SHA-256 |
|---|---|
| `NF-DOS6.0.img` | `6A6E80534F07AA86C621F40696DCD11D83A7236AE013371BBF76BF8F9E41618B` |
| `NF-DOS6.0.iso` | `BF247F1C0BB256FF5169844D307E33D9B6AE3837AD5C2FEC43199DB94112B420` |
| `UIDE.SYS` | `D340BFB3544C150B2D4938B1B4B75F78F58E1B95B91E2E7DB3A03131B2EA01ED` |
| `UDVD2.SYS` | `748A22D72E4245EB176BA6F6DD264A2BA571634D320FFF19297791D5D1A57B5E` |

Os pacotes oficiais completos, com fontes, binários, documentação e avisos upstream, acompanham o arquivo de fontes em `packages/uide.zip` e `packages/udvd2.zip`. UIDE tem 57.170 bytes e SHA-1 `B3DBDCC5118E19AD1474BBA09768F33797186F58`; UDVD2 tem 35.734 bytes e SHA-1 `C7AED9F82327AACB5D5ADBA8BF3A7EFB21B623A0`. A comparação confirma identidade com o pacote upstream, não compilação independente.

O splash do disquete mantém o nome da empresa **Panthera Softwares**. O diretório e os títulos da suíte no ISO usam **Panthera Works 1.0**. Avisos e datas de copyright de terceiros foram mantidos. Os nomes públicos e de download permanecem `NF-DOS 6.0`, `NF-DOS6.0.img` e `NF-DOS6.0.iso`; os hashes documentam os novos bytes.

QEMU 8.2.2 iniciou ambas as imagens até a tela NF-DOS 6.0 e o prompt `A:\>`. O arquivo FAT12 passou na verificação de integridade, e o ISO foi relido com o catálogo El Torito e o boot image presentes. Esta verificação confirma inicialização e integridade do pacote, não substitui uma auditoria técnica integral do conteúdo ISO nem resolve os itens abertos já listados.
