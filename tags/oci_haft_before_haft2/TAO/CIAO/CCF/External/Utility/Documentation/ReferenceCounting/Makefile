# $Id$
# file      : Documentation/ReferenceCounting/Make.doc.mk
# author    : Boris Kolpackov <boris@kolpackov.net>
# copyright : Copyright (c) 2002-2003 Boris Kolpackov
# license   : http://kolpackov.net/license.html

root := ../..

include $(root)/Config/Documentation.pre.rules

vpath %.hpp $(root)/Utility/ReferenceCounting

doc_translation_units := DefaultImpl.hpp \
                         Interface.hpp   \
		         SmartPtr.hpp    \
			 StrictPtr.hpp

DOC_FLAGS := --strip-preprocessor -css $(root)/Documentation/Style/CXX.css

include $(root)/Config/Documentation.post.rules
