//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_typecode.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for TypeCodes for various types
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_typecode.h"

// include all the individual files
#include "be_visitor_typecode/typecode_decl.cpp"
#include "be_visitor_typecode/typecode_defn.cpp"

ACE_RCSID(be, be_visitor_typecode, "$Id$")

