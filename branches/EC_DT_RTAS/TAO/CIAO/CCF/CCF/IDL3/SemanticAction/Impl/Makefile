#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

include ../../../../Config.rules

LIBNAME  = libPhony
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES = Component   \
        Consumes    \
        Emits       \
        EventType   \
        Factory     \
        Home        \
        HomeFactory \
        HomeFinder  \
        Include     \
        Provides    \
        Publishes   \
        Uses 

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


.obj/Component.o .obj/Component.o .obj/Component.o .obj/Component.o: Component.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Component.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Component.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Consumes.o .obj/Consumes.o .obj/Consumes.o .obj/Consumes.o: Consumes.cpp ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Consumes.hpp \
  ../../../../CCF/IDL3/SemanticGraph/EventType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Consumes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Emits.o .obj/Emits.o .obj/Emits.o .obj/Emits.o: Emits.cpp ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Emits.hpp \
  ../../../../CCF/IDL3/SemanticGraph/EventType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Emits.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/EventType.o .obj/EventType.o .obj/EventType.o .obj/EventType.o: EventType.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/EventType.hpp \
  ../../../../CCF/IDL3/SemanticGraph/EventType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/EventType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Factory.o .obj/Factory.o .obj/Factory.o .obj/Factory.o: Factory.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Factory.hpp \
  ../../../../CCF/IDL3/SemanticAction/Factory.hpp \
  ../../../../CCF/IDL2/SemanticAction/Factory.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Factory.hpp \
  ../../../../CCF/CompilerElements/Diagnostic.hpp \
  ../../../../CCF/CompilerElements/ExH.hpp \
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
  ../../../../CCF/IDL2/SemanticAction/ValueType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Component.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Component.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Provides.hpp \
  ../../../../CCF/IDL3/SemanticAction/Provides.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Uses.hpp \
  ../../../../CCF/IDL3/SemanticAction/Uses.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Publishes.hpp \
  ../../../../CCF/IDL3/SemanticGraph/EventType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Publishes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Emits.hpp \
  ../../../../CCF/IDL3/SemanticAction/Emits.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Consumes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Consumes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/EventType.hpp \
  ../../../../CCF/IDL3/SemanticAction/EventType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Home.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Home.hpp \
  ../../../../CCF/IDL3/SemanticAction/Home.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFactory.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFinder.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Include.hpp

.obj/Home.o .obj/Home.o .obj/Home.o .obj/Home.o: Home.cpp ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Home.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Home.hpp \
  ../../../../CCF/IDL3/SemanticAction/Home.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/HomeFactory.o .obj/HomeFactory.o .obj/HomeFactory.o .obj/HomeFactory.o: HomeFactory.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Home.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFactory.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/HomeFinder.o .obj/HomeFinder.o .obj/HomeFinder.o .obj/HomeFinder.o: HomeFinder.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Home.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Operation.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Exception.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Interface.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFinder.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Include.o .obj/Include.o .obj/Include.o .obj/Include.o: Include.cpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Include.hpp \
  ../../../../CCF/CompilerElements/Context.hpp \
  ../../../../CCF/CompilerElements/Diagnostic.hpp \
  ../../../../CCF/CompilerElements/ExH.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Graph.tpp \
  ../../../../CCF/CompilerElements/Introspection.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Name.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Elements.tpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Include.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/SemanticAction/Factory.hpp \
  ../../../../CCF/IDL2/SemanticAction/Factory.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../../../../CCF/CompilerElements/TokenStream.hpp \
  ../../../../CCF/CompilerElements/Preprocessor.hpp \
  ../../../../CCF/CompilerElements/PreprocessorToken.hpp \
  ../../../../CCF/IDL3/LexicalAnalyzer.hpp \
  ../../../../CCF/IDL2/LexicalAnalyzer.hpp \
  ../../../../CCF/IDL3/Parser.hpp ../../../../CCF/IDL2/Parser.hpp \
  ../../../../CCF/IDL2/SemanticAction.hpp \
  ../../../../CCF/IDL2/SemanticAction/Attribute.hpp \
  ../../../../CCF/IDL2/SemanticAction/Enum.hpp \
  ../../../../CCF/IDL2/SemanticAction/Exception.hpp \
  ../../../../CCF/IDL2/SemanticAction/Interface.hpp \
  ../../../../CCF/IDL2/SemanticAction/Member.hpp \
  ../../../../CCF/IDL2/SemanticAction/Module.hpp \
  ../../../../CCF/IDL2/SemanticAction/Native.hpp \
  ../../../../CCF/IDL2/SemanticAction/Operation.hpp \
  ../../../../CCF/IDL2/SemanticAction/Struct.hpp \
  ../../../../CCF/IDL2/SemanticAction/Typedef.hpp \
  ../../../../CCF/IDL2/SemanticAction/TypeId.hpp \
  ../../../../CCF/IDL2/SemanticAction/Union.hpp \
  ../../../../CCF/IDL2/SemanticAction/ValueType.hpp \
  ../../../../CCF/IDL2/Parsing/Elements.hpp \
  ../../../../CCF/IDL2/Parsing/Action.hpp \
  ../../../../CCF/IDL2/Parsing/Recovery.hpp \
  ../../../../CCF/IDL3/SemanticAction.hpp \
  ../../../../CCF/IDL3/SemanticAction/Component.hpp \
  ../../../../CCF/IDL3/SemanticAction/Consumes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Emits.hpp \
  ../../../../CCF/IDL3/SemanticAction/EventType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Home.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFactory.hpp \
  ../../../../CCF/IDL3/SemanticAction/HomeFinder.hpp \
  ../../../../CCF/IDL3/SemanticAction/Provides.hpp \
  ../../../../CCF/IDL3/SemanticAction/Publishes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Uses.hpp

.obj/Provides.o .obj/Provides.o .obj/Provides.o .obj/Provides.o: Provides.cpp ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Provides.hpp \
  ../../../../CCF/IDL3/SemanticAction/Provides.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Publishes.o .obj/Publishes.o .obj/Publishes.o .obj/Publishes.o: Publishes.cpp \
  ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Publishes.hpp \
  ../../../../CCF/IDL3/SemanticGraph/EventType.hpp \
  ../../../../CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../../../../CCF/IDL3/SemanticAction/Publishes.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

.obj/Uses.o .obj/Uses.o .obj/Uses.o .obj/Uses.o: Uses.cpp ../../../../CCF/IDL3/SemanticGraph/Component.hpp \
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
  ../../../../CCF/IDL3/SemanticGraph/Elements.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Uses.hpp \
  ../../../../CCF/IDL3/SemanticAction/Uses.hpp \
  ../../../../CCF/IDL3/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Elements.hpp \
  ../../../../CCF/IDL2/Token.hpp \
  ../../../../CCF/CompilerElements/ReferenceCounting.hpp \
  ../../../../CCF/IDL3/Token.hpp \
  ../../../../CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.hpp \
  ../../../../CCF/CompilerElements/FileSystem.hpp \
  ../../../../CCF/IDL2/SemanticGraph/Translation.ipp \
  ../../../../CCF/IDL2/SemanticAction/Impl/Elements.tpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
