//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_typedef.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Typedef
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_typedef.h"

// include all the individual files
#include "be_visitor_typedef/typedef.cpp"
#include "be_visitor_typedef/typedef_ch.cpp"
#include "be_visitor_typedef/typedef_ci.cpp"
#include "be_visitor_typedef/typedef_cs.cpp"
#include "be_visitor_typedef/any_op_ch.cpp"
#include "be_visitor_typedef/any_op_cs.cpp"
#include "be_visitor_typedef/cdr_op_ch.cpp"
#include "be_visitor_typedef/cdr_op_ci.cpp"
#include "be_visitor_typedef/cdr_op_cs.cpp"

ACE_RCSID(be, be_visitor_typedef, "$Id$")
