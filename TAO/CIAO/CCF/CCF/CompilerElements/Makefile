#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../Config.rules

LIBNAME  = libCompilerElements
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = Introspection     \
        PreprocessorToken \
        Preprocessor

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


.obj/Introspection.o .obj/Introspection.o .obj/Introspection.o .obj/Introspection.o: Introspection.cpp

.obj/PreprocessorToken.o .obj/PreprocessorToken.o .obj/PreprocessorToken.o .obj/PreprocessorToken.o: PreprocessorToken.cpp \
  ../../CCF/CompilerElements/PreprocessorToken.hpp

.obj/Preprocessor.o .obj/Preprocessor.o .obj/Preprocessor.o .obj/Preprocessor.o: Preprocessor.cpp \
  ../../CCF/CompilerElements/Preprocessor.hpp \
  ../../CCF/CompilerElements/TokenStream.hpp \
  ../../CCF/CompilerElements/PreprocessorToken.hpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
