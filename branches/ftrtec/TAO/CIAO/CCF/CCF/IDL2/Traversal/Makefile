#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../../Config.rules

LIBNAME  = libPhony
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = Attribute    \
        Elements     \
        Enum         \
        Exception    \
        Fundamental  \
        Interface    \
        Member       \
        Module       \
        Native       \
        Operation    \
        Sequence     \
        Struct       \
        Translation  \
        TypeId       \
        Union        \
        ValueType

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


.obj/Attribute.o .obj/Attribute.o .obj/Attribute.o .obj/Attribute.o: Attribute.cpp ../../../CCF/IDL2/Traversal/Attribute.hpp \
  ../../../CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp

.obj/Elements.o .obj/Elements.o .obj/Elements.o .obj/Elements.o: Elements.cpp ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp

.obj/Enum.o .obj/Enum.o .obj/Enum.o .obj/Enum.o: Enum.cpp ../../../CCF/IDL2/Traversal/Enum.hpp \
  ../../../CCF/IDL2/SemanticGraph/Enum.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp

.obj/Exception.o .obj/Exception.o .obj/Exception.o .obj/Exception.o: Exception.cpp ../../../CCF/IDL2/Traversal/Exception.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Exception.hpp

.obj/Fundamental.o .obj/Fundamental.o .obj/Fundamental.o .obj/Fundamental.o: Fundamental.cpp \
  ../../../CCF/IDL2/Traversal/Fundamental.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Fundamental.hpp

.obj/Interface.o .obj/Interface.o .obj/Interface.o .obj/Interface.o: Interface.cpp ../../../CCF/IDL2/Traversal/Interface.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../CCF/IDL2/Traversal/Interface.tpp

.obj/Member.o .obj/Member.o .obj/Member.o .obj/Member.o: Member.cpp ../../../CCF/IDL2/Traversal/Member.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Member.hpp

.obj/Module.o .obj/Module.o .obj/Module.o .obj/Module.o: Module.cpp ../../../CCF/IDL2/Traversal/Module.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Module.hpp

.obj/Native.o .obj/Native.o .obj/Native.o .obj/Native.o: Native.cpp ../../../CCF/IDL2/Traversal/Native.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Native.hpp

.obj/Operation.o .obj/Operation.o .obj/Operation.o .obj/Operation.o: Operation.cpp ../../../CCF/IDL2/Traversal/Operation.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../CCF/IDL2/Traversal/Operation.tpp

.obj/Sequence.o .obj/Sequence.o .obj/Sequence.o .obj/Sequence.o: Sequence.cpp ../../../CCF/IDL2/Traversal/Sequence.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Sequence.hpp

.obj/Struct.o .obj/Struct.o .obj/Struct.o .obj/Struct.o: Struct.cpp ../../../CCF/IDL2/Traversal/Struct.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Struct.hpp

.obj/Translation.o .obj/Translation.o .obj/Translation.o .obj/Translation.o: Translation.cpp \
  ../../../CCF/IDL2/Traversal/Translation.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../CCF/IDL2/SemanticGraph/Translation.ipp

.obj/TypeId.o .obj/TypeId.o .obj/TypeId.o .obj/TypeId.o: TypeId.cpp ../../../CCF/IDL2/Traversal/TypeId.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../../../CCF/IDL2/SemanticGraph/Literals.hpp

.obj/Union.o .obj/Union.o .obj/Union.o .obj/Union.o: Union.cpp ../../../CCF/IDL2/Traversal/Union.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/Union.hpp \
  ../../../CCF/IDL2/SemanticGraph/Member.hpp

.obj/ValueType.o .obj/ValueType.o .obj/ValueType.o .obj/ValueType.o: ValueType.cpp ../../../CCF/IDL2/Traversal/ValueType.hpp \
  ../../../CCF/IDL2/Traversal/Elements.hpp \
  ../../../CCF/CompilerElements/Introspection.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../CCF/CompilerElements/Context.hpp \
  ../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../CCF/IDL2/Traversal/Elements.tpp \
  ../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../CCF/IDL2/Traversal/ValueType.tpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
