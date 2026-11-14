# NFSPLASH — source

`NFSPLASH.ASM` draws the NF-DOS logo on the active VGA text page. `generate.py` converts `reference/def-splash.png` into the raster embedded in the DOS program. The generator requires Python 3 and NASM; the resulting `.COM` image is a build output and is not versioned here.

Example, from the repository root:

```powershell
python src/nfsplash/generate.py --image src/nfsplash/reference/def-splash.png --nasm nasm --output NFSPLASH.COM
```

The code is under MIT. The project owner also authorized distribution of the NF-DOS artwork supplied for this publication under MIT. See [LICENSE-SCOPE.md](../../LICENSE-SCOPE.md).
