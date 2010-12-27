set MAYA_VERSION=2009

REM Copy the plug-in binary (maya2osg.mll)
set PLUGIN_DIRECTORY="%PROGRAMFILES%\Autodesk\Maya%MAYA_VERSION%\bin\plug-ins\"
if exist %MAYA_PLUG_IN_PATH% set PLUGIN_DIRECTORY=%MAYA_PLUGIN_PATH%
copy bin\maya2osg.mll %PLUGIN_DIRECTORY%

REM Copy plug-in associated files (MEL scripts, shelf buttons, ...)
if exist "%USERPROFILE%\Documents" GOTO ELSE
	if exist "%USERPROFILE%\My Documents" GOTO MYDOCS
		set DOCFILES="%USERPROFILE%\Mis Documentos"
		goto ENDIF
:MYDOCS
		set DOCFILES="%USERPROFILE%\My Documents"
		goto ENDIF	
:ELSE
	set DOCFILES="%USERPROFILE%\Documents"
:ENDIF

copy ..\scripts\*.mel %DOCFILES%\maya\%MAYA_VERSION%\scripts
copy ..\prefs\icons\*.bmp %DOCFILES%\maya\%MAYA_VERSION%\prefs\icons
copy ..\prefs\shelves\*.mel %DOCFILES%\maya\%MAYA_VERSION%\prefs\shelves
