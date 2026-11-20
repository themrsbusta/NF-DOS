# NFSPLASH — source

`NFSPLASH.ASM` draws the NF-DOS logo on the active VGA text page. `generate.py` converts `reference/def-splash.png` into the raster embedded in the DOS program. The generator requires Python 3 and NASM; the resulting `.COM` image is a build output and is not versioned here.

Example, from the repository root:

```powershell
python src/nfsplash/generate.py --image src/nfsplash/reference/def-splash.png --nasm nasm --output NFSPLASH.COM
```

The code is under MIT. The artwork is a separate work and is not covered by that license. I, Busta, created the original NF-DOS logo. Panthera Softwares is my company and holds the rights to the artwork; Panthera authorizes its use and redistribution as part of NF-DOS 6.0. See [LICENSE-SCOPE.md](../../LICENSE-SCOPE.md) for the scope of that authorization.
