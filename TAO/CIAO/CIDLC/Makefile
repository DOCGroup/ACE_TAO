# File   : Makefile
# Author : Boris Kolpackov <boris@kolpackov.net>
# $Id$

root = CCF/External/Utility

include $(root)/Config/Executable.pre.rules

cxx_translation_units := ExecutorMappingGenerator.cpp   \
         		 ServantGenerator.cpp           \
                         ServantHeaderGenerator.cpp     \
                         ServantSourceGenerator.cpp     \
                         cidlc.cpp

module_base           := cidlc
module_prefix         :=
module_suffix         :=

CXX_PREPROCESS_FLAGS  += -ICCF  -ICCF/External/Utility -ICCF/External/boost

CXX_LINK_LIBS         += -LCCF/CCF/IDL2               \
                         -lIDL2                       \
                         -LCCF/CCF/IDL3               \
                         -lIDL3                       \
         		 -LCCF/CCF/CIDL               \
                         -lCIDL                       \
        		 -LCCF/CCF/CodeGenerationKit  \
        		 -lCodeGenerationKit          \
        		 -LCCF/External/boost/lib     \
                         -lboost_regex                \
                         -lboost_filesystem

include $(root)/Config/Executable.post.rules
