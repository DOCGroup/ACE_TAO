#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../Config.rules

LIBNAME  = libCodeGenerationKit
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = CommandLine         \
        CommandLineGrammar  \
        CommandLineParser

LSRC = $(addsuffix .cpp,$(FILES))

override static_libs_only := true

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

CPPFLAGS += -I../..

ifneq ($(MAKECMDGOALS),depend)
CPPFLAGS += $(UTILITY_CPP_FLAGS) $(BOOST_CPP_FLAGS)
endif

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/CommandLine.o .obj/CommandLine.o .obj/CommandLine.o .obj/CommandLine.o: CommandLine.cpp \
  ../../CCF/CodeGenerationKit/CommandLine.hpp

.obj/CommandLineGrammar.o .obj/CommandLineGrammar.o .obj/CommandLineGrammar.o .obj/CommandLineGrammar.o: CommandLineGrammar.cpp \
  ../../CCF/CodeGenerationKit/CommandLineGrammar.hpp \
  ../../CCF/CodeGenerationKit/CommandLine.hpp

.obj/CommandLineParser.o .obj/CommandLineParser.o .obj/CommandLineParser.o .obj/CommandLineParser.o: CommandLineParser.cpp \
  ../../CCF/CodeGenerationKit/CommandLineParser.hpp \
  ../../CCF/CodeGenerationKit/CommandLine.hpp \
  ../../CCF/CodeGenerationKit/CommandLineGrammar.hpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
