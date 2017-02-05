@echo off 
IF "%QT_DIR%" == "" (
	echo Set QT_DIR as env var!
	goto DONE
)

"%QT_DIR%\bin\qdoc.exe" --single-exec master.qdocconf
echo Finished doc generation.

:DONE
pause

