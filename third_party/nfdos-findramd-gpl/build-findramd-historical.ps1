$ErrorActionPreference='Stop'
$root=(Get-Location).Path
$source=Join-Path $root 'sources\nfdos-findramd'
$work=Join-Path $root 'diagnostics\beta2-build\work\findramd-final'
New-Item -ItemType Directory -Force -Path $work | Out-Null
Copy-Item -LiteralPath (Join-Path $source 'FINDRAMD.C') -Destination $work -Force
$env:WATCOM=Join-Path $root 'tools\watcom2026'
$env:INCLUDE=Join-Path $env:WATCOM 'h'
$env:PATH=(Join-Path $env:WATCOM 'binnt')+';'+$env:PATH
Set-Location $work
& (Join-Path $env:WATCOM 'binnt\wcl.exe') '-bt=DOS' '-mt' '-0' '-os' '-s' '-zp1' 'FINDRAMD.C' '-fe=FINDRAMD.EXE'
if($LASTEXITCODE -ne 0){throw "FINDRAMD compile failed: $LASTEXITCODE"}
Copy-Item -LiteralPath (Join-Path $work 'FINDRAMD.EXE') -Destination (Join-Path $root 'diagnostics\beta2-build\overlay\FINDRAMD.EXE') -Force
