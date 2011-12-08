@REM $Id$

@if "%1a" == "a" goto error
@if "%2a" == "a" goto error
release\ft_replica -o %1.ior -q -f none -l %1 -i %2 %3 %4 %5 %6 %7 %8 %9
@goto end
@:error
@echo Usage %0 location role [options]
@:end
pause
exit
