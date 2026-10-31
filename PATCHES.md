# Alterações locais

As alterações listadas abaixo fazem parte da versão Beta 2 e continuam sob as licenças dos componentes upstream. Os patches textuais estão ao lado deste arquivo.

- **FreeDOS Kernel 2046** (`patches/freedos-kernel-nfdos.patch`): configuração silenciosa do banner de inicialização, remoção da pausa F5/F8 e supressão da mensagem de ausência de discos rígidos.
- **FreeCOM 0.87** (`patches/freecom-nfdos.patch`): renomeia `toUpper` para `supplToUpper` nos pontos de colisão. A correção evita recursão involuntária e a corrupção de memória que aparecia como `E_NoMem`.
- **EDIT**: os cinco arquivos-fonte usados pelo Beta 2 v8 estão em `third_party/edit/source/`: `console.c`, `edit.c`, `menubar.c`, `message.c` e `video.c`. Incluem ajustes de compatibilidade de compilação e o retorno correto ao DOS em Alt+X/Alt+F4. O executável resultante não está incluído.

As fontes do EDIT partem do commit registrado em `SOURCES.md`. O patch compilado foi testado na imagem de diagnóstico v8; esse resultado não transforma esta publicação de fontes em um build integralmente reproduzível.
