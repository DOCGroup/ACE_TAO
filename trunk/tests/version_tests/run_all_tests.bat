@echo off

:dd
if not exist "DLL Debug" goto dr
cd "DLL Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dr
if not exist "DLL Release" goto dud
cd "DLL Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dud
if not exist "DLL Unicode Debug" goto dur
cd "DLL Unicode Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dur
if not exist "DLL Unicode Release" goto sd
cd "DLL Unicode Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sd
if not exist "static Debug" goto sr
cd "static Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sr
if not exist "static Release" goto sud
cd "static Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sud
if not exist "static Unicode Debug" goto sur
cd "static Unicode Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sur
if not exist "static Unicode Release" goto done
cd "static Unicode Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:done