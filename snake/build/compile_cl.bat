@ECHO OFF

del *.obj *.pdb

REM COMPILE
cl /nologo /Zi /GR- /EHs /W4 /MDd -I ..\include -I ..\deps\esat\include /c ..\src\*.cc

REM LINK
cl /nologo /Zi /GR- /EHs /W4 /MDd /Fe:test.exe *.obj ..\deps\esat\bin\ESAT_d.lib opengl32.lib user32.lib gdi32.lib shell32.lib

pause