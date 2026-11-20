@echo off
if "%1" == "" goto end
goto %1

:1
\Kernel\Startup\NFSPLASH.COM
@echo.
@echo.
@echo NF-DOS 6.0
@echo PANTHERA WORKS 1.0
goto end

:2
\Kernel\Startup\NFSPLASH.COM
@echo.
@echo.
@echo NF-DOS 6.0
@echo PANTHERA WORKS 1.0

:end
