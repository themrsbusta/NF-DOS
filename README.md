# NF-DOS 6.0 Beta 2 — fontes

NF-DOS foi meu primeiro projeto próprio. Comecei em 2006, usando como base o disco de inicialização do Windows 98 SE. Em 2026, retomei o projeto para celebrar seus 20 anos, preservar a identidade do NF-DOS e substituir o núcleo por tecnologia FreeDOS.

Esta publicação contém **somente fontes, documentação, avisos de licença e a arte original usada como entrada do splash**. Não contém a imagem de disquete nem programas compilados (`.COM`, `.EXE`, `.SYS`, `.IMG` ou arquivos equivalentes).

## O que está aqui

- Fonte do FreeDOS Kernel 2046 e do FreeCOM, com as alterações locais usadas no Beta 2.
- Fontes dos utilitários FreeDOS e de terceiros listados em [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), cada qual sob seus próprios termos.
- Código Panthera para `NFSPLASH.COM` e `FINDRAMD`, scripts de inicialização/instalação e o logo-fonte `def-splash.png`.
- Revisões upstream, alterações locais e limitações de build documentadas em [SOURCES.md](SOURCES.md), [PATCHES.md](PATCHES.md) e [BUILDING.md](BUILDING.md).

O kernel usado é o FreeDOS Kernel 2046 (v2.46), identificado pelo upstream como release de 29 de junho de 2026: [release oficial](https://github.com/FDOS/kernel/releases/tag/ke2046). O shell é FreeCOM 0.87.

## Limites desta publicação

Este é um pacote de fontes para os componentes e scripts NF-DOS identificados. **Ainda não é uma receita reproduzível, de ponta a ponta, para recriar byte a byte o disquete Beta 2.** A imagem de referência, o setor de boot histórico e os binários usados na integração foram deliberadamente omitidos. O repositório também não afirma que todo arquivo legado presente na imagem beta tenha uma origem de fonte identificada.

O objetivo desta publicação é manter as fontes identificadas e seus avisos, sem distribuir binários. Veja os limites e dependências em [BUILDING.md](BUILDING.md).

## Licenças

Os arquivos originais Panthera neste repositório são disponibilizados sob MIT, nos limites definidos em [LICENSE-SCOPE.md](LICENSE-SCOPE.md). **MIT não substitui nem relicencia código de terceiros.** Para componentes externos, valem os avisos e licenças preservados em cada diretório de `third_party/` e descritos em [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Pastas

- `src/boot/` — scripts e configuração NF-DOS.
- `src/findramd/`, `src/nfsplash/` — código original Panthera.
- `third_party/` — snapshots de fontes upstream, sem o histórico Git aninhado e sem saídas compiladas.
- `patches/` — alterações locais em componentes de terceiros; permanecem sob as licenças upstream aplicáveis.

Contribuições e correções são bem-vindas. Antes de incluir novos componentes, preserve a licença original e registre a revisão de origem.
