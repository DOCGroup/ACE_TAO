#----------------------------------------------------------------------------
#       $Id$
#
#----------------------------------------------------------------------------

include ../CCF/Config.rules

BIN     = cidlc

FILES   = ExecutorMappingGenerator \
          DescriptorGenerator      \
          SizeTypeCalculator       \
          ServantGenerator         \
          ServantHeaderGenerator   \
          ServantSourceGenerator   \
          TypeNameEmitter          \
          cidlc

SRC     = $(addsuffix .cpp,$(FILES))

BUILD   = $(VBIN)

INSTALL = $(VBIN:%=$(INSBIN)/%$(EXEEXT))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

CPPFLAGS += -I.. -I../CCF

ifneq ($(MAKECMDGOALS),depend)
CPPFLAGS += $(UTILITY_CPP_FLAGS) $(BOOST_CPP_FLAGS)
endif

LIBS += -L../CCF/CCF/CIDL               \
        -lCIDL                          \
        -L../CCF/CCF/IDL3               \
        -lIDL3                          \
        -L../CCF/CCF/IDL2               \
        -lIDL2                          \
        -L../CCF/CCF/CodeGenerationKit  \
        -lCodeGenerationKit             \
        -L../CCF/CCF/CompilerElements   \
        -lCompilerElements              \
        $(BOOST_LD_FLAGS)               \
        -lboost_regex                   \
        -lboost_filesystem

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/ExecutorMappingGenerator.o .obj/ExecutorMappingGenerator.so .shobj/ExecutorMappingGenerator.o .shobj/ExecutorMappingGenerator.so: ExecutorMappingGenerator.cpp \
  ExecutorMappingGenerator.hpp ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineDescriptor.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp \
  ../CCF/CCF/CodeGenerationKit/Regex.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationIDL.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationBuffer.hpp \
  ../CCF/CCF/CompilerElements/ExH.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationImplanter.hpp

.obj/DescriptorGenerator.o .obj/DescriptorGenerator.so .shobj/DescriptorGenerator.o .shobj/DescriptorGenerator.so: DescriptorGenerator.cpp DescriptorGenerator.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineDescriptor.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp Literals.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp \
  ../CCF/CCF/CodeGenerationKit/Regex.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationXML.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationBuffer.hpp \
  ../CCF/CCF/CompilerElements/ExH.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationImplanter.hpp

.obj/SizeTypeCalculator.o .obj/SizeTypeCalculator.so .shobj/SizeTypeCalculator.o .shobj/SizeTypeCalculator.so: SizeTypeCalculator.cpp SizeTypeCalculator.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp Literals.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp

.obj/ServantGenerator.o .obj/ServantGenerator.so .shobj/ServantGenerator.o .shobj/ServantGenerator.so: ServantGenerator.cpp ServantGenerator.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineDescriptor.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp ServantHeaderGenerator.hpp \
  ServantSourceGenerator.hpp ../CCF/CCF/CodeGenerationKit/Regex.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationCxx.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationBuffer.hpp \
  ../CCF/CCF/CompilerElements/ExH.hpp \
  ../CCF/CCF/CodeGenerationKit/IndentationImplanter.hpp

.obj/ServantHeaderGenerator.o .obj/ServantHeaderGenerator.so .shobj/ServantHeaderGenerator.o .shobj/ServantHeaderGenerator.so: ServantHeaderGenerator.cpp \
  ServantHeaderGenerator.hpp ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp TypeNameEmitter.hpp Literals.hpp \
  ../CCF/CCF/CodeGenerationKit/Regex.hpp

.obj/ServantSourceGenerator.o .obj/ServantSourceGenerator.so .shobj/ServantSourceGenerator.o .shobj/ServantSourceGenerator.so: ServantSourceGenerator.cpp \
  ServantSourceGenerator.hpp ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp Literals.hpp TypeNameEmitter.hpp \
  ../CCF/CCF/CodeGenerationKit/Regex.hpp

.obj/TypeNameEmitter.o .obj/TypeNameEmitter.so .shobj/TypeNameEmitter.o .shobj/TypeNameEmitter.so: TypeNameEmitter.cpp TypeNameEmitter.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp Literals.hpp

.obj/cidlc.o .obj/cidlc.so .shobj/cidlc.o .shobj/cidlc.so: cidlc.cpp ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/CompilerElements/Diagnostic.hpp \
  ../CCF/CCF/CompilerElements/ExH.hpp \
  ../CCF/CCF/CompilerElements/TokenStream.hpp \
  ../CCF/CCF/CompilerElements/Preprocessor.hpp \
  ../CCF/CCF/CompilerElements/PreprocessorToken.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineParser.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineDescriptor.hpp \
  ../CCF/CCF/CIDL/LexicalAnalyzer.hpp ../CCF/CCF/IDL3/LexicalAnalyzer.hpp \
  ../CCF/CCF/IDL2/LexicalAnalyzer.hpp ../CCF/CCF/IDL2/Token.hpp \
  ../CCF/CCF/CompilerElements/ReferenceCounting.hpp \
  ../CCF/CCF/CIDL/Parser.hpp ../CCF/CCF/IDL3/Parser.hpp \
  ../CCF/CCF/IDL2/Parser.hpp ../CCF/CCF/IDL2/SemanticAction.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Include.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Member.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Module.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Native.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Typedef.hpp \
  ../CCF/CCF/IDL2/SemanticAction/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Union.hpp \
  ../CCF/CCF/IDL2/SemanticAction/ValueType.hpp \
  ../CCF/CCF/IDL2/Parsing/Elements.hpp ../CCF/CCF/IDL2/Parsing/Action.hpp \
  ../CCF/CCF/IDL2/Parsing/Recovery.hpp ../CCF/CCF/IDL3/SemanticAction.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Elements.hpp ../CCF/CCF/IDL3/Token.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Component.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Consumes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Emits.hpp \
  ../CCF/CCF/IDL3/SemanticAction/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Home.hpp \
  ../CCF/CCF/IDL3/SemanticAction/HomeFactory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/HomeFinder.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Provides.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Publishes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Uses.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Factory.hpp \
  ../CCF/CCF/CIDL/SemanticAction.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Elements.hpp ../CCF/CCF/CIDL/Token.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticAction/HomeExecutor.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Factory.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Include.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Member.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Module.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Native.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Typedef.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Union.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Component.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Provides.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Uses.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Publishes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Emits.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Consumes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Home.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Include.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp \
  ExecutorMappingGenerator.hpp ServantGenerator.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp DescriptorGenerator.hpp \
  SizeTypeCalculator.hpp ../ciao/Version.h

.obj/cidlc.o .obj/cidlc.so .shobj/cidlc.o .shobj/cidlc.so: cidlc.cpp ../CCF/CCF/CompilerElements/Context.hpp \
  ../CCF/CCF/CompilerElements/FileSystem.hpp \
  ../CCF/CCF/CompilerElements/Diagnostic.hpp \
  ../CCF/CCF/CompilerElements/ExH.hpp \
  ../CCF/CCF/CompilerElements/TokenStream.hpp \
  ../CCF/CCF/CompilerElements/Preprocessor.hpp \
  ../CCF/CCF/CompilerElements/PreprocessorToken.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLine.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineParser.hpp \
  ../CCF/CCF/CodeGenerationKit/CommandLineDescriptor.hpp \
  ../CCF/CCF/CIDL/LexicalAnalyzer.hpp ../CCF/CCF/IDL3/LexicalAnalyzer.hpp \
  ../CCF/CCF/IDL2/LexicalAnalyzer.hpp ../CCF/CCF/IDL2/Token.hpp \
  ../CCF/CCF/CompilerElements/ReferenceCounting.hpp \
  ../CCF/CCF/CIDL/Parser.hpp ../CCF/CCF/IDL3/Parser.hpp \
  ../CCF/CCF/IDL2/Parser.hpp ../CCF/CCF/IDL2/SemanticAction.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Include.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Member.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Module.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Native.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Typedef.hpp \
  ../CCF/CCF/IDL2/SemanticAction/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Union.hpp \
  ../CCF/CCF/IDL2/SemanticAction/ValueType.hpp \
  ../CCF/CCF/IDL2/Parsing/Elements.hpp ../CCF/CCF/IDL2/Parsing/Action.hpp \
  ../CCF/CCF/IDL2/Parsing/Recovery.hpp ../CCF/CCF/IDL3/SemanticAction.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Elements.hpp ../CCF/CCF/IDL3/Token.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Component.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Consumes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Emits.hpp \
  ../CCF/CCF/IDL3/SemanticAction/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Home.hpp \
  ../CCF/CCF/IDL3/SemanticAction/HomeFactory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/HomeFinder.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Provides.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Publishes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Uses.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Factory.hpp \
  ../CCF/CCF/CIDL/SemanticAction.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Elements.hpp ../CCF/CCF/CIDL/Token.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticAction/HomeExecutor.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Factory.hpp \
  ../CCF/CCF/CIDL/SemanticGraph.hpp ../CCF/CCF/IDL3/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Graph.tpp \
  ../CCF/CCF/CompilerElements/Introspection.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Name.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticGraph/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Fundamental.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Literals.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Member.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Module.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Native.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Sequence.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Translation.ipp \
  ../CCF/CCF/IDL2/SemanticGraph/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/Union.hpp \
  ../CCF/CCF/IDL2/SemanticGraph/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Component.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticGraph/Home.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticGraph/Executor.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Factory.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Elements.tpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Attribute.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Enum.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Exception.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Include.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Interface.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Member.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Module.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Native.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Operation.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Struct.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Typedef.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/TypeId.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/Union.hpp \
  ../CCF/CCF/IDL2/SemanticAction/Impl/ValueType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Component.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Provides.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Uses.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Publishes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Emits.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Consumes.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/EventType.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Home.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp \
  ../CCF/CCF/IDL3/SemanticAction/Impl/Include.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Composition.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/Elements.hpp \
  ../CCF/CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp \
  ExecutorMappingGenerator.hpp ServantGenerator.hpp \
  ../CCF/CCF/CIDL/Traversal.hpp ../CCF/CCF/IDL3/Traversal.hpp \
  ../CCF/CCF/IDL2/Traversal.hpp ../CCF/CCF/IDL2/Traversal/Elements.hpp \
  ../CCF/CCF/IDL2/Traversal/Elements.tpp \
  ../CCF/CCF/IDL2/Traversal/Attribute.hpp \
  ../CCF/CCF/IDL2/Traversal/Enum.hpp \
  ../CCF/CCF/IDL2/Traversal/Exception.hpp \
  ../CCF/CCF/IDL2/Traversal/Fundamental.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.hpp \
  ../CCF/CCF/IDL2/Traversal/Interface.tpp \
  ../CCF/CCF/IDL2/Traversal/Member.hpp \
  ../CCF/CCF/IDL2/Traversal/Module.hpp \
  ../CCF/CCF/IDL2/Traversal/Native.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.hpp \
  ../CCF/CCF/IDL2/Traversal/Operation.tpp \
  ../CCF/CCF/IDL2/Traversal/Struct.hpp \
  ../CCF/CCF/IDL2/Traversal/Sequence.hpp \
  ../CCF/CCF/IDL2/Traversal/Translation.hpp \
  ../CCF/CCF/IDL2/Traversal/TypeId.hpp \
  ../CCF/CCF/IDL2/Traversal/Union.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.hpp \
  ../CCF/CCF/IDL2/Traversal/ValueType.tpp \
  ../CCF/CCF/IDL3/Traversal/Elements.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.hpp \
  ../CCF/CCF/IDL3/Traversal/Component.tpp \
  ../CCF/CCF/IDL3/Traversal/EventType.hpp \
  ../CCF/CCF/IDL3/Traversal/EventType.tpp \
  ../CCF/CCF/IDL3/Traversal/Home.hpp \
  ../CCF/CCF/CIDL/Traversal/Elements.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.hpp \
  ../CCF/CCF/CIDL/Traversal/Composition.tpp \
  ../CCF/CCF/CIDL/Traversal/Executor.hpp DescriptorGenerator.hpp \
  SizeTypeCalculator.hpp ../ciao/Version.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
