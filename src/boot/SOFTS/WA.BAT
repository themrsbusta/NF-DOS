@echo off
if "%1" == "" goto end
md %1\softs
md %1\kernel\temp
ren C:\kernel\commands\short.zoi *.bat
@echo @echo off >%1\kernel\commands\setsys.bat
@echo set hd=%1>%1\kernel\commands\setsys.bat