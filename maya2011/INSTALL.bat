set MAYA_VERSION=2011

REM Copy the plug-in binary (maya2osg.mll)
copy bin\maya2osg.mll "%PROGRAMFILES%\Autodesk\Maya%MAYA_VERSION%\bin\plug-ins\"

REM Copy plug-in associated files (MEL scripts, shelf buttons, ...)
if exist "%USERPROFILE%\Documents" GOTO ELSE
	set DOCFILES="%USERPROFILE%\My Documents"
:ELSE
	set DOCFILES="%USERPROFILE%\Documents"
:ENDIF

copy scripts\*.mel %DOCFILES%\maya\%MAYA_VERSION%\scripts
copy prefs\icons\*.bmp %DOCFILES%\maya\%MAYA_VERSION%\prefs\icons
copy prefs\shelves\*.mel %DOCFILES%\maya\%MAYA_VERSION%\prefs\shelves
