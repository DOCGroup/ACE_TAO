#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../../Config.rules

LIBNAME  = libPhony
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = Elements Executor

LSRC = $(addsuffix .cpp,$(FILES))

override static_libs_only := true

.PHONY: objects

objects:

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

objects: $(VSHOBJS)

CPPFLAGS += -I../../..

ifneq ($(MAKECMDGOALS),depend)
CPPFLAGS += $(UTILITY_CPP_FLAGS) $(BOOST_CPP_FLAGS)
endif

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Elements.o .obj/Elements.o .obj/Elements.o .obj/Elements.o: Elements.cpp ../../../CCF/CIDL/Traversal/Elements.hpp \
  ../../../CCF/IDL3/Traversal/Elements.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/CIDL/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL3/SemanticGraph/Elements.hpp

.obj/Executor.o .obj/Executor.o .obj/Executor.o .obj/Executor.o: Executor.cpp ../../../CCF/CIDL/Traversal/Executor.hpp \
  ../../../CCF/CIDL/SemanticGraph/Executor.hpp \
  ../../../CCF/IDL3/SemanticGraph/Component.hpp \
  ../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL3/SemanticGraph/Home.hpp \
  ../../../CCF/CIDL/SemanticGraph/Elements.hpp \
  ../../../CCF/CIDL/Traversal/Elements.hpp \
  ../../../CCF/IDL3/Traversal/Elements.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
