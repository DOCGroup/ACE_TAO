# File   : Makefile
# Author : Boris Kolpackov <boris@kolpackov.net>
# $Id$

root = ../../External/Utility

include $(root)/Config/Archive.pre.rules

cxx_translation_units := IDL2.cpp                      \
                         IDL2_LexicalAnalyzer.cpp      \
                         IDL2_SyntaxTree.cpp           \
                         IDL2_SyntaxTree_BuiltIn.cpp   \
                         IDL2_Traversal.cpp

module_base           := IDL2
module_prefix         :=
module_suffix         :=

CXX_PREPROCESS_FLAGS  += -I../..                          \
                         -I../../External/Utility         \
                         -I../../External/boost           \

include $(root)/Config/Archive.post.rules
