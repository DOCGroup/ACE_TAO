//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_array.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Array
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_array.h"

// include all the individual files
#include "be_visitor_array/array.cpp"
#include "be_visitor_array/array_ch.cpp"
#include "be_visitor_array/array_ci.cpp"
#include "be_visitor_array/array_cs.cpp"
#include "be_visitor_array/any_op_ch.cpp"
#include "be_visitor_array/any_op_cs.cpp"
#include "be_visitor_array/cdr_op_ch.cpp"
#include "be_visitor_array/cdr_op_ci.cpp"
#include "be_visitor_array/cdr_op_cs.cpp"

ACE_RCSID(be, be_visitor_array, "$Id$")
