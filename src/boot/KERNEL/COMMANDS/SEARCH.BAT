@echo off
@echo System Search for DOS
@echo.
@echo.
@echo.
IF "%1" == "" goto ?
dir/s/w/p/o %1
goto end

:?
@echo Enter the file name to search for, including drive and extension.
@echo.
@echo Example: [DRIVE:\FILE.EXTENSION]
@echo.  
@echo If you do not know part of the name, replace it with *.
@echo.
@echo Example: [DRIVE:\*ILE.EXT*ION]
goto end

:END

