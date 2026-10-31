@echo off
cls
@echo NF-DOS installer
@echo.
IF "%1" == "" goto ?
@echo copying NF-DOS in %1\, press Ctrl+C to cancel or enter to continue
pause 
@echo starting copy
copy \ %1\ >nul
cls
@echo NF-DOS installer
@echo.
@echo --->----------- 25%
sys A: %1 >nul
md %1\kernel >nul
cls
@echo NF-DOS installer
@echo.
@echo ------->------- 50%
md %1\kernel\App >nul
md %1\kernel\Commands >nul
md %1\kernel\NFD >nul
md %1\kernel\Startup >nul
cls
@echo NF-DOS installer
@echo.
@echo ----------->--- 75%
copy \kernel\App %1\kernel\App >nul
copy \kernel\Commands %1\kernel\Commands >nul
copy \kernel\NFD %1\kernel\NFD >nul
copy \kernel\Startup %1\kernel\Startup >nul
call \softs\wa.bat %1
del %1\autoexec.bat
copy 2autoe.bat %1\autoexec.bat >nul
cls
@echo NF-DOS installer
@echo.
@echo Complete install in %1.
del %1\install.bat >nul
del %1\2autoe.bat >nul
goto end

:?
@echo Para iniciar a instalação digite a unidade.
@echo Exemplo: [install unidade:]
goto end

:end