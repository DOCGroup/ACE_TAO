@echo off
rem    To use this either give it no arguments to run all the tests or
rem    pass it the test name (without the extention) to run only one
rem    test

if not "%1" == "" goto runtest

call run_tests 

call run_tests Address_Test
call run_tests Counter64_Test
call run_tests Counter_Test
call run_tests Gauge_Test
call run_tests Integer_Test
call run_tests Octet_Test
call run_tests Oid_Test
call run_tests Target_Test
call run_tests Varbind_Test

goto done

:runtest

echo Running %1
%1.exe
if errorlevel 0 goto fine
echo.
echo %1 has FAILED!!!
echo.
type %temp%\log\%1.log | find /I "assertion failed"
type %temp%\log\%1.log | find /I "not supported"
type %temp%\log\%1.log | find /I "no such file or directory"
type %temp%\log\%1.log | find /I "invalid argument"
type %temp%\log\%1.log | find /I "timeout"
type %temp%\log\%1.log | find /I "bad file number"
echo.

goto done
:fine

rem We should check the log files here to make sure the test ended correctly
rem type %temp%\log\%1.log | find "Ending"

:done
