// file      : Test/ExH/Inline/inline.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

//
// This is a link-time test to detect any problems with inline functions
// (notably missing inline specifier).

/* FUZZ: disable check_for_improper_main_declaration */

#include "Utility/ExH/ExH.hpp"

int
main ()
{
}

//$Id$
