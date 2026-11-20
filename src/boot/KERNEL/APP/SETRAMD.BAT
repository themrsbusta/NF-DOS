@echo off
set RAMD=
set CDROM=
\Kernel\NFD\BITDISK.EXE 4096 >NUL
\Kernel\App\findramd >NUL
if errorlevel 255 goto no_ramdrive
if not errorlevel 3 goto no_ramdrive
if errorlevel 27 goto no_ramdrive
if errorlevel 26 goto ram26
if errorlevel 25 goto ram25
if errorlevel 24 goto ram24
if errorlevel 23 goto ram23
if errorlevel 22 goto ram22
if errorlevel 21 goto ram21
if errorlevel 20 goto ram20
if errorlevel 19 goto ram19
if errorlevel 18 goto ram18
if errorlevel 17 goto ram17
if errorlevel 16 goto ram16
if errorlevel 15 goto ram15
if errorlevel 14 goto ram14
if errorlevel 13 goto ram13
if errorlevel 12 goto ram12
if errorlevel 11 goto ram11
if errorlevel 10 goto ram10
if errorlevel 9 goto ram9
if errorlevel 8 goto ram8
if errorlevel 7 goto ram7
if errorlevel 6 goto ram6
if errorlevel 5 goto ram5
if errorlevel 4 goto ram4
if errorlevel 3 goto ram3
goto no_ramdrive
:ram26
set RAMD=Z
set CDROM=*
goto selected
:ram25
set RAMD=Y
set CDROM=Z
goto selected
:ram24
set RAMD=X
set CDROM=Y
goto selected
:ram23
set RAMD=W
set CDROM=X
goto selected
:ram22
set RAMD=V
set CDROM=W
goto selected
:ram21
set RAMD=U
set CDROM=V
goto selected
:ram20
set RAMD=T
set CDROM=U
goto selected
:ram19
set RAMD=S
set CDROM=T
goto selected
:ram18
set RAMD=R
set CDROM=S
goto selected
:ram17
set RAMD=Q
set CDROM=R
goto selected
:ram16
set RAMD=P
set CDROM=Q
goto selected
:ram15
set RAMD=O
set CDROM=P
goto selected
:ram14
set RAMD=N
set CDROM=O
goto selected
:ram13
set RAMD=M
set CDROM=N
goto selected
:ram12
set RAMD=L
set CDROM=M
goto selected
:ram11
set RAMD=K
set CDROM=L
goto selected
:ram10
set RAMD=J
set CDROM=K
goto selected
:ram9
set RAMD=I
set CDROM=J
goto selected
:ram8
set RAMD=H
set CDROM=I
goto selected
:ram7
set RAMD=G
set CDROM=H
goto selected
:ram6
set RAMD=F
set CDROM=G
goto selected
:ram5
set RAMD=E
set CDROM=F
goto selected
:ram4
set RAMD=D
set CDROM=E
goto selected
:ram3
set RAMD=C
set CDROM=D
goto selected
:selected
if "%RAMD%"=="C" goto c_drive
goto success
:c_drive
@echo No fixed hard disk detected; RAM disk assigned to C:.
goto success
:no_ramdrive
cls
call \kernel\Startup\vis.bat 1
:success
