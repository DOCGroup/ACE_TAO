@echo off
REM # ====================================================================
REM #
REM # OpenCCM: The Open CORBA Component Model Platform
REM # Copyright (C) 2000-2003 INRIA - USTL - LIFL - GOAL
REM # Contact: openccm@objectweb.org
REM #
REM # This library is free software; you can redistribute it and/or
REM # modify it under the terms of the GNU Lesser General Public
REM # License as published by the Free Software Foundation; either
REM # version 2.1 of the License, or any later version.
REM #
REM # This library is distributed in the hope that it will be useful,
REM # but WITHOUT ANY WARRANTY; without even the implied warranty of
REM # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
REM # Lesser General Public License for more details.
REM #
REM # You should have received a copy of the GNU Lesser General Public
REM # License along with this library; if not, write to the Free Software
REM # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
REM # USA
REM #
REM # Initial developer(s): Romain Rouvoy.
REM # Contributor(s): Philippe Merle.
REM #
REM # ====================================================================
REM #
REM # Starting the Ant Compilation Process.
REM #
REM # ====================================================================

setlocal

if "%JAVA_HOME%"=="" goto :javahome_error

REM Added for Bug #468.
REM Contributor: Philippe Merle.

if "%OpenCCM_HOMEDIR%"=="" goto :openccm_homedir_error

REM Add end.

REM Updated for Bug #468.
REM Contributor: Philippe Merle.

REM set JAVA=%JAVA_HOME%\bin\java
REM set CP=@CLASSPATH@
REM %JAVA% -classpath "%CP%" -Dant.home=@ANT_HOME@ org.apache.tools.ant.Main %1 %2 %3 %4 %5 %6 -q -buildfile build.xml

set CP=%JAVA_HOME%\lib\tools.jar;%OpenCCM_HOMEDIR%\externals\xerces.jar;%CLASSPATH%
for %%i in (%OpenCCM_HOMEDIR%\externals\ant\*.jar) do call %OpenCCM_HOMEDIR%\bin\cp.bat %%i

%JAVA_HOME%\bin\java -classpath "%CP%" -Dant.home=%OpenCCM_HOMEDIR%\externals\ant org.apache.tools.ant.Main %1 %2 %3 %4 %5 %6 -q -buildfile build.xml

REM Update end.

goto :eof

:javahome_error
echo Cannot find JAVA. Please set your JAVA_HOME environment variable.

REM Added for Bug #468.
REM Contributor: Philippe Merle.

:openccm_homedir_error
echo Please set the OpenCCM_HOMEDIR environment variable.
goto :eof
