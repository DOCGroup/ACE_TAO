cd "DLL Debug"
call ..\..\run_tests.bat %*
cd ..
cd "DLL Release"
call ..\..\run_tests.bat %*
cd ..
cd "DLL Unicode Debug"
call ..\..\run_tests.bat %*
cd ..
cd "DLL Unicode Release"
call ..\..\run_tests.bat %*
cd ..
cd "static Debug"
call ..\..\run_tests.bat %*
cd ..
cd "static Release"
call ..\..\run_tests.bat %*
cd ..
cd "static Unicode Debug"
call ..\..\run_tests.bat %*
cd ..
cd "static Unicode Release"
call ..\..\run_tests.bat %*
cd ..
