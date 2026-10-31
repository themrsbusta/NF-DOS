@echo off
if "%1" == "" goto see

:setting
set sysver=6.0 BETA 2 [FreeDOS Kernel 2046]
set edition=FULL EDITION
set name=NF-DOS
set produtor=PANTHERA SOFTWARES
set year=2026
goto end

:see
@echo %name% %sysver%
@echo %produtor% %year%
@echo.
@echo Kernel:
ver

:end
