@echo off

:dd
if not exist "DLL Debug" goto dr
echo.
echo ** DLL Debug **
echo.
cd "DLL Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dr
if not exist "DLL Release" goto dud
echo.
echo ** DLL Release **
echo.
cd "DLL Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dud
if not exist "DLL Unicode Debug" goto dur
echo.
echo ** DLL Unicode Debug **
echo.
cd "DLL Unicode Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:dur
if not exist "DLL Unicode Release" goto sd
echo.
echo ** DLL Unicode Release **
echo.
cd "DLL Unicode Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sd
if not exist "static Debug" goto sr
echo.
echo ** static Debug **
echo.
cd "static Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sr
if not exist "static Release" goto sud
echo.
echo ** static Release **
echo.
cd "static Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sud
if not exist "static Unicode Debug" goto sur
echo.
echo ** static Unicode Debug **
echo.
cd "static Unicode Debug"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:sur
if not exist "static Unicode Release" goto done
echo.
echo ** static Unicode Release **
echo.
cd "static Unicode Release"
call ..\..\run_tests.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..

:done