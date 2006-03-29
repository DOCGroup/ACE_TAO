# file      : Example/Introspection/InheritanceTree/Makefile
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../../..

include $(root)/BuildRules/Bootstrap.rules

$(call include, $(root)/BuildRules/Executable.pre.rules)


cxx_translation_units := Hierarchy.cpp inheritance_tree.cpp

module_base           := inheritance_tree

CXX_PREPROCESS_FLAGS  += -I $(root)

CXX_LINK_LIBS         += -L$(root)/Utility/Introspection -lIntrospection

$(call include, $(root)/BuildRules/Executable.post.rules)
# $Id$
