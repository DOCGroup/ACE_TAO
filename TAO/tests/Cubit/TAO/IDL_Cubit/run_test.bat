@echo off
perl -I..\..\..\bin run_test.pl %1 %2 %3 %4 %5 %6 %7 %8 %9

rem If there were errors (like perl wasn't found) echo this message
if errorlevel 1 echo.
if errorlevel 1 echo You need to have Perl for Win32 installed to run this script.
if errorlevel 1 echo It can be found at http://www.activestate.com/pw32/
if errorlevel 1 echo Also remember to add the perl\doc directory to the PERLLIB environment variable.
