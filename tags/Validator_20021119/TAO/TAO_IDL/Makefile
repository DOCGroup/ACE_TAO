#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(CROSS-COMPILE),)
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
endif # ! CROSS-COMPILE

#----------------------------------------------------------------------------
#       Define local rules and targets
#---------------------------------------------------------------------------

ifneq ($(CROSS-COMPILE),)
  all default_embedded:
	@echo The IDL compiler won\'t run or build on CROSS-COMPILE platforms.

  clean realclean:
	@echo Nothing to clean in TAO_IDL on CROSS-COMPILE platforms.
else  # ! CROSS-COMPILE

MKLIST = \
	Makefile.FE \
	Makefile.BE \
	Makefile.EXE \
	Makefile.dependencies

## If parallel makes (-j) are invoked , we want to make sure that the order
## of the Makefiles invoked from this Makefile is preserved.
## Recursively invoked makes from this file will still be run in parallel.
.NOTPARALLEL:

## Makefile.BE.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.BE to be invoked
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)

all debug profile optimize install deinstall clean realclean clobber depend idl_stubs :
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif


endif # ! CROSS-COMPILE

fe/fe_lookup.cpp: fe/keywords.dat
	$(RM) fe/fe_lookup.cpp
	echo "// \$$Id\$$" > fe/fe_lookup.cpp
	echo "#include	\"idl.h\"" >> fe/fe_lookup.cpp
	echo "#include	\"idl_extern.h\"" >> fe/fe_lookup.cpp
	echo "#include	\"fe_private.h\"" >> fe/fe_lookup.cpp
	$(ACE_ROOT)/bin/gperf -M -J -c -C -D -E -T -f 0 -a -o -t -p -K keyword_  \
			-L C++ -Z TAO_IDL_CPP_Keyword_Table -N lookup -k1,2,$$ fe/keywords.dat \
			 >> fe/fe_lookup.cpp

fe/lex.yy.cpp:	fe/idl.ll
	@echo "WARNING: Potentially outdated fe/lex.yy.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make lex.yy.cpp.target"

.PHONY: lex.yy.cpp.target

# For DOC group use
YACC = /home/cs/faculty/schmidt/bin/SunOS5/yacc

lex.yy.cpp.target:
		/pkg/gnu2k1/bin/flex -L -t fe/idl.ll | \
		sed -e "s/yy/tao_yy/g" \
		    -e "s/YY/TAO_YY/g" \
		    -e "s@#include <unistd\.h>@#include \"ace/OS.h\"@" \
		    -e "/#include </d" \
		    -e "s@ECHO@TAO_YY_ECHO@" \
		    -e 's/\$$Hea''der.*\$$/$$I''d$$/' > fe/lex.yy.cpp
		(cd fe ; patch < lex.yy.cpp.diff)

fe/y.tab.cpp:	fe/idl.yy
	@echo "WARNING: Potentially outdated fe/y.tab.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make y.tab"

.PHONY: y.tab

y.tab: fe/idl.yy
		$(YACC) -l -d fe/idl.yy
		sed -e 's/char \*getenv/char *ace_foo/g' \
		    -e 's/= getenv/= ACE_OS::getenv/g' \
		    -e 's/int yynew_state,/int,/g' \
		    -e 's/int yyold_state,/int,/g' \
		    -e 's/int yyrule_num,/int,/g' \
		    -e 's/\(YYDEBUG_DISCARD_TOKEN.*int\) yytoken_num/\1/g' \
		    -e 's/yynewerror://g' \
		    -e 's/yy/tao_yy/g' \
		    -e 's/YY/TAO_YY/g' \
		    -e 's/y\.tab\.c/y.tab.cpp/g' \
		    -e 's%\(#pragma ident\)%// \1%' < y.tab.c > /tmp/$(USER).Y
		sed -e 's/yy/tao_yy/g' \
		    -e 's/YY/TAO_YY/g' < y.tab.h > /tmp/$(USER).YH
		echo '// $$I''d$$' > fe/y.tab.cpp
		cat /tmp/$(USER).Y >> fe/y.tab.cpp
		echo '// $$I''d$$' > fe/y.tab.h
		cat /tmp/$(USER).YH >> fe/y.tab.h
		$(RM) /tmp/$(USER).Y /tmp/$(USER).YH y.tab.c y.tab.h
		(cd fe ; patch < y.tab.cpp.diff)

tags:
	$(RM) TAGS
	find . -name "*.h" -print | etags -a --c++ -
	find . -name "*.cpp" -print | etags -a -


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
