# File   : Makefile
# Author : Boris Kolpackov <boris@kolpackov.net>
# $Id$

root = .

include $(root)/CCF/Bootstrap.rules

$(call include, $(root)/CCF/Config.rules)

$(call include, $(UTILITY_ROOT)/BuildRules/Executable.pre.rules)

cxx_translation_units := ExecutorMappingGenerator.cpp   \
			 ServantGenerator.cpp           \
	                 ServantHeaderGenerator.cpp     \
                         ServantSourceGenerator.cpp     \
                         cidlc.cpp

module_base           := cidlc
module_prefix         :=
module_suffix         :=

CXX_PREPROCESS_FLAGS  += -I$(root)/CCF  -I$(UTILITY_ROOT) -I$(BOOST_ROOT)

CXX_LINK_LIBS         += -L$(root)/CCF/CCF/IDL2               \
                         -lIDL2                       \
                         -L$(root)/CCF/CCF/IDL3               \
                         -lIDL3                       \
			 -L$(root)/CCF/CCF/CIDL               \
                         -lCIDL                       \
			 -L$(root)/CCF/CCF/CodeGenerationKit  \
			 -lCodeGenerationKit          \
			 -L$(BOOST_ROOT)/lib     \
                         -lboost_regex                \
                         -lboost_filesystem	      \
			 -L$(UTILITY_ROOT)/Utility/Introspection \
			 -lIntrospection

$(call include, $(UTILITY_ROOT)/BuildRules/Executable.post.rules)
