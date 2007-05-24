# file      : Test/ReferenceCounting/Interface/Makefile
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../../..

include $(root)/BuildRules/Bootstrap.rules

$(call include, $(root)/BuildRules/Executable.pre.rules)


cxx_translation_units := interface.cpp

module_base           := interface
module_prefix         :=
module_suffix         :=


CXX_PREPROCESS_FLAGS  += -I $(root)


$(call include, $(root)/BuildRules/Executable.post.rules)
# $Id$
