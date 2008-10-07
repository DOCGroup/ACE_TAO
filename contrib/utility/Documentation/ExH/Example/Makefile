# file      : Documentation/ExH/Example/Makefile
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../../..

include $(root)/BuildRules/Bootstrap.rules

$(call include, $(root)/BuildRules/Documentation.pre.rules)

vpath %.cpp $(root)/Example/ExH/BadCast
vpath %.cpp $(root)/Example/ExH/Compound
vpath %.cpp $(root)/Example/ExH/HelloWorld
vpath %.cpp $(root)/Example/ExH/LogicToSystem

doc_translation_units := bad_cast.cpp        \
                         compound.cpp        \
                         hello_world.cpp     \
                         logic_to_system.cpp

DOC_FLAGS := -css $(root)/Documentation/Style/CXX.css

$(call include, $(root)/BuildRules/Documentation.post.rules)

# $Id$
