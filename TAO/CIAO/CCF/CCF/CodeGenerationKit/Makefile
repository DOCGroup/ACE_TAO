# File   : Makefile
# Author : Boris Kolpackov <boris@kolpackov.net>
# $Id$

root = ../../External/Utility

include $(root)/Config/Archive.pre.rules

cxx_translation_units := CodeGenerationKit.cpp   \
                         CommandLine.cpp         \
                         CommandLineGrammar.cpp  \
                         CommandLineParser.cpp

module_base           := CodeGenerationKit
module_prefix         := 
module_suffix         :=

CXX_PREPROCESS_FLAGS  += -I../..                          \
                         -I../../External/Utility         \
                         -I../../External/boost           \

include $(root)/Config/Archive.post.rules
