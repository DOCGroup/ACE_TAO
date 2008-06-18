# file      : Utility/Introspection/Makefile
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../..

include $(root)/BuildRules/Bootstrap.rules

$(call include, $(root)/BuildRules/Archive.pre.rules)

cxx_translation_units := TypeId.cpp Object.cpp

module_base           := Introspection

CXX_PREPROCESS_FLAGS  += -I $(root)

$(call include, $(root)/BuildRules/Archive.post.rules)
# $Id$
