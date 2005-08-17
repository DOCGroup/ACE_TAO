# file      : Documentation/ExH/System/Makefile
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../../..

include $(root)/BuildRules/Bootstrap.rules

$(call include, $(root)/BuildRules/Documentation.pre.rules)

vpath %.hpp $(root)/Utility/ExH/System

doc_translation_units := Exception.hpp DescriptiveException.hpp

DOC_FLAGS := --strip-preprocessor -css $(root)/Documentation/Style/CXX.css

$(call include, $(root)/BuildRules/Documentation.post.rules)

# $Id$
