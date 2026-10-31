# Building

## Fontes e ferramentas

Este repositório não inclui compiladores, assemblers, programas DOS compilados nem imagens. As fontes upstream mantêm seus próprios arquivos de build e instruções. O caminho do NFSPLASH está documentado em `src/nfsplash/README.md` e requer Python 3 e NASM.

Os demais componentes foram compilados no ambiente original com ferramentas como Open Watcom, NASM e JWasm, conforme os próprios projetos. Instale versões compatíveis e siga a documentação upstream incluída em cada pasta. As ferramentas não fazem parte desta publicação; suas referências e licenças estão em `THIRD_PARTY_NOTICES.md`.

## Reconstrução do disquete

A reconstrução integral da imagem Beta 2 **não é reproduzível somente com este repositório**. A cadeia histórica depende de uma imagem-base e de um setor de boot binário que não estão incluídos, além de entradas compiladas cuja correspondência fonte/binário ainda não foi totalmente verificada. Esses artefatos foram omitidos para manter esta publicação source-only e não redistribuir material legado sem proveniência completa.

Por isso, não use esta árvore como prova de que a imagem `.IMG` foi construída integralmente a partir das fontes presentes. O objetivo desta versão é publicar os fontes identificados, patches, scripts NF-DOS e avisos de licenciamento com proveniência explícita.
