#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../../../Config.rules

LIBNAME  = libPhony
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = Attribute  \
        Elements   \
        Enum       \
        Exception  \
        Factory    \
        Include    \
        Interface  \
        Member     \
        Module     \
        Native     \
        Operation  \
        Struct     \
        TypeId     \
        Typedef    \
        Union      \
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

CPPFLAGS += -I../../../..

ifneq ($(MAKECMDGOALS),depend)
CPPFLAGS += $(UTILITY_CPP_FLAGS) $(BOOST_CPP_FLAGS)
endif

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Attribute.o .obj/Attribute.o .obj/Attribute.o .obj/Attribute.o: Attribute.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Elements.o .obj/Elements.o .obj/Elements.o .obj/Elements.o: Elements.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Enum.o .obj/Enum.o .obj/Enum.o .obj/Enum.o: Enum.cpp ../../../../CCF/IDL2/SemanticAction/Impl/Enum.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Enum.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Enum.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Exception.o .obj/Exception.o .obj/Exception.o .obj/Exception.o: Exception.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Exception.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Factory.o .obj/Factory.o .obj/Factory.o .obj/Factory.o: Factory.cpp ../../../../CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Factory.hpp \
  ../../../../CCF/CompilerElements/Diagnostic.hpp \
  ../../../../CCF/CompilerElements/ExH.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Factory.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Enum.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Enum.hpp \
  ../../../../CCF/IDL2/SemanticAction/Enum.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Include.hpp \
  ../../../../CCF/IDL2/SemanticAction/Include.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Interface.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticAction/Interface.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Module.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Module.hpp \
  ../../../../CCF/IDL2/SemanticAction/Module.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Native.hpp \
  ../../../../CCF/IDL2/SemanticAction/Native.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Operation.hpp \
  ../../../../CCF/IDL2/SemanticAction/Operation.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Struct.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Struct.hpp \
  ../../../../CCF/IDL2/SemanticAction/Struct.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Typedef.hpp \
  ../../../../CCF/IDL2/SemanticAction/Typedef.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticAction/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Union.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Union.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Union.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticAction/ValueType.hpp

.obj/Include.o .obj/Include.o .obj/Include.o .obj/Include.o: Include.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Include.hpp \
  ../../../../CCF/IDL2/SemanticAction/Include.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Interface.o .obj/Interface.o .obj/Interface.o .obj/Interface.o: Interface.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Interface.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Interface.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Member.o .obj/Member.o .obj/Member.o .obj/Member.o: Member.cpp ../../../../CCF/IDL2/SemanticAction/Impl/Member.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Member.hpp

.obj/Module.o .obj/Module.o .obj/Module.o .obj/Module.o: Module.cpp ../../../../CCF/IDL2/SemanticAction/Impl/Module.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Module.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Module.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Native.o .obj/Native.o .obj/Native.o .obj/Native.o: Native.cpp ../../../../CCF/IDL2/SemanticGraph/Native.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Native.hpp \
  ../../../../CCF/IDL2/SemanticAction/Native.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Operation.o .obj/Operation.o .obj/Operation.o .obj/Operation.o: Operation.cpp \
  ../../../../CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Operation.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Struct.o .obj/Struct.o .obj/Struct.o .obj/Struct.o: Struct.cpp ../../../../CCF/IDL2/SemanticAction/Impl/Struct.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Struct.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Struct.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/TypeId.o .obj/TypeId.o .obj/TypeId.o .obj/TypeId.o: TypeId.cpp ../../../../CCF/IDL2/SemanticAction/Impl/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticAction/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Literals.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Module.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp

.obj/Typedef.o .obj/Typedef.o .obj/Typedef.o .obj/Typedef.o: Typedef.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Typedef.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/Typedef.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Sequence.hpp

.obj/Union.o .obj/Union.o .obj/Union.o .obj/Union.o: Union.cpp ../../../../CCF/IDL2/SemanticAction/Impl/Union.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Union.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Union.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Enum.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Fundamental.hpp

.obj/ValueType.o .obj/ValueType.o .obj/ValueType.o .obj/ValueType.o: ValueType.cpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticAction/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
