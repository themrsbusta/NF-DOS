@echo off
@echo System Search for DOS
@echo.
@echo.
@echo.
IF "%1" == "" goto ?
dir/s/w/p/o %1
goto end

:?
@echo digite o nome do arquivo a localizar com unidade e extensao
@echo.
@echo exemplo [UNIDADE:\ARQUIVO.EXTENSAO]
@echo.  
@echo quando o que nao souber, complete com *
@echo.
@echo exemplo [UNIDADE:\*IVO.EXTE*AO]
goto end

:END

