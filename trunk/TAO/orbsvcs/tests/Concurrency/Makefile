#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the concurrency service test.
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lorbsvcs -lTAO

PROG_SRCS = CC_client.cpp CC_tests.cpp CC_naming_service.cpp \
                tmplinst.cpp CC_command.tab.cpp \
                lex.CC_command.cpp CC_command.cpp CC_test_utils.cpp

LSRC = $(PROG_SRCS)

CC_CLT_OBJS = CC_client.o CC_tests.o tmplinst.o CC_naming_service.o \
                CC_command.tab.o lex.CC_command.o \
                CC_command.o CC_test_utils.o

VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

BIN2 = CC_client

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Concurrency,$(findstring Concurrency,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Concurrency

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

lex.CC_command.cpp:     CC_command.l CC_command.tab.h
	@echo "WARNING: Potentially outdated lex.CC_command.cpp"
	@echo "         either touch(1) the file or explicitly"
	@echo "         generate it using:"
	@echo "    make lex.CC_comand.cpp.target"

.PHONY: lex.CC_command.cpp.target

lex.CC_command.cpp.target:
		$(LEX) -t -I CC_command.l > /tmp/$(USER).CC_L1
		sed -e "s/unistd/stdio/g" \
	        -e "s/\\$$\(Header[^$$]*\) \\$$/\\1/" \
		-e "s/yy/ace_cc_yy/g" \
		-e "s/YY/ACE_CC_YY/g" \
		-e "s/free( ptr );/free( ACE_MALLOC_T (ptr) );/g" \
		-e "s/realloc( ptr, size );/realloc( ACE_MALLOC_T (ptr), size );/g" \
		\
		-e "s@#include <stdio\.h>@#include \"ace/OS.h\"@" \
		-e "s@#include <@#include /**/ <@" \
		-e "s@ECHO@ACE_CC_ECHO@" < /tmp/$(USER).CC_L1 > /tmp/$(USER).CC_L2
		cp /tmp/$(USER).CC_L2 lex.CC_command.cpp
		patch < lex.CC_command.cpp.diff
		$(RM) -f /tmp/$(USER).CC_L1 /tmp/$(USER).CC_L2

CC_command.tab.cpp CC_command.tab.h: CC_command.y
	@echo "WARNING: Potentially outdated CC_command.tab.cpp"
	@echo "         either touch(1) the file or explicitly "
	@echo "         generate it using:"
	@echo "    make CC_comand.tab"

.PHONY: CC_command.tab

CC_command.tab: CC_command.y
		$(YACC) -d CC_command.y
		sed -e "s/char \*getenv/char *ace_foo/g" \
		    -e "s/= getenv/= ACE_OS::getenv/g" \
		    -e "s/yynewerror://g" \
		    -e "s/yy/ace_cc_yy/g" \
		    -e "s/YY/ACE_CC_YY/g" \
		    -e "s/y\.tab\.c/CC_command.tab.cpp/g" < y.tab.c > /tmp/$(USER).CC_Y
		sed -e "s/yy/ace_cc_yy/g" \
		    -e "s/YY/ACE_CC_YY/g" < y.tab.h > /tmp/$(USER).CC_YH
		cp /tmp/$(USER).CC_Y CC_command.tab.cpp
		cp /tmp/$(USER).CC_YH CC_command.tab.h
		$(RM) -f /tmp/$(USER).CC_Y  /tmp/$(USER).CC_YH y.tab.c y.tab.h

CC_client: $(addprefix $(VDIR),$(CC_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
