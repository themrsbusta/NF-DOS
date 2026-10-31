@ECHO OFF
set TZ=UTC0
call \Kernel\Commands\sysver.bat setting
set DIRCMD=/O:N
set LglDrv=27 * 26 Z 25 Y 24 X 23 W 22 V 21 U 20 T 19 S 18 R 17 Q 16 P 15
set LglDrv=%LglDrv% O 14 N 13 M 12 L 11 K 10 J 9 I 8 H 7 G 6 F 5 E 4 D 3 C
call \Kernel\App\setramd.bat
set temp=%RAMD%:\TEMP
set tmp=%RAMD%:\TEMP
md %RAMD%:\APP >nul
md %RAMD%:\COMMANDS >nul
md %RAMD%:\TEMP >nul
copy kernel\app %RAMD%:\app >nul
copy kernel\commands %RAMD%:\commands >nul
copy command.com %RAMD%:\ > NUL
set comspec=%RAMD%:\command.com
\Kernel\App\UNZIP.EXE -o -qq A:\NFAPPS.ZIP -d %RAMD%:\ > NUL
path=%RAMD%:\;%RAMD%:\commands;%RAMD%:\app
LH %RAMD%:\APP\CTMOUSE.EXE >NUL
CALL MSCDEX /D:mscd001 /L:%CDROM% >NUL
call \kernel\Startup\vis.bat 2
