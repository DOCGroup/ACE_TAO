# File   : Makefile
# Author : Boris Kolpackov <boris@kolpackov.net>
# $Id$

root = ../../External/Utility

include $(root)/Config/Archive.pre.rules

cxx_translation_units := CIDL.cpp                       \
                         CIDL_SyntaxTree.cpp            \
                         CIDL_Traversal.cpp

module_base           := CIDL
module_prefix         :=
module_suffix         :=

CXX_PREPROCESS_FLAGS  += -I../..                          \
                         -I../../External/Utility         \
                         -I../../External/boost           \

include $(root)/Config/Archive.post.rules
