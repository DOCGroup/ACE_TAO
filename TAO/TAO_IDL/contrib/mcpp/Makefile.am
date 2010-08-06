## Process this file with automake to produce Makefile.in

if MCPP_LIB

lib_LTLIBRARIES = libmcpp.la
libmcpp_la_SOURCES = system.H configed.H internal.H main.c directive.c eval.c expand.c mbchar.c support.c system.c lib.c mcpp_lib.h
libmcpp_la_LDFLAGS = -no-undefined -version-info 0:1:0

main.lo directive.lo eval.lo expand.lo mbohar.lo support.lo system.lo	: system.H internal.H mcpp_lib.h
lib.lo      :   configed.H
system.H    :   configed.H

else

bin_PROGRAMS = mcpp
mcpp_SOURCES = system.H configed.H internal.H main.c directive.c eval.c expand.c mbchar.c support.c system.c lib.c mcpp_lib.h
mcpp_CFLAGS = $(AM_CFLAGS)		## dummy to avoid conflict with MCPP_LIB

main.o directive.o eval.o expand.o mbohar.o support.o system.o    : system.H internal.H mcpp_lib.h
lib.o       :   configed.H
system.H    :   configed.H

endif

if REPLACE_CPP
if GNUC

if MINGW
cc1_PROGRAMS = cc1
cc1_SOURCES = cc1.c
cc1dir = $(bindir)
endif

install-exec-hook:
	@if test x"$(gcc_path)" = x""; then	exit 1; fi
	@$(srcdir)/set_mcpp.sh '$(gcc_path)' $(gcc_maj_ver) $(gcc_min_ver) '$(cpp_call)' '$(CC)' '$(CXX)' 'x$(EXEEXT)' '$(LN_S)' '$(inc_dir)'
uninstall-hook:
	@$(srcdir)/unset_mcpp.sh '$(gcc_path)' $(gcc_maj_ver) $(gcc_min_ver) '$(cpp_call)' '$(CC)' '$(CXX)' 'x$(EXEEXT)' '$(LN_S)' '$(inc_dir)'
endif
endif
