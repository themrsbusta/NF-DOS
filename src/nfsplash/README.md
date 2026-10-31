# NFSPLASH — fonte

`NFSPLASH.ASM` desenha o logo NF-DOS na página de texto VGA ativa. `generate.py` converte `reference/def-splash.png` para a rasterização embutida no programa DOS. O gerador precisa de Python 3 e NASM; a imagem `.COM` resultante é uma saída de compilação e não é versionada aqui.

Exemplo, a partir da raiz do repositório:

```powershell
python src/nfsplash/generate.py --image src/nfsplash/reference/def-splash.png --nasm nasm --output NFSPLASH.COM
```

O código está sob MIT. O titular do projeto autorizou que a arte NF-DOS fornecida para esta publicação também seja distribuída sob MIT. Veja [LICENSE-SCOPE.md](../../LICENSE-SCOPE.md).
