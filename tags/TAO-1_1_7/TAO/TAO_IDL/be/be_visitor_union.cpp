//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_union.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Union
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_union.h"

// include all the individual files
#include "be_visitor_union/discriminant_ch.cpp"
#include "be_visitor_union/discriminant_ci.cpp"
#include "be_visitor_union/discriminant_cs.cpp"
#include "be_visitor_union/union.cpp"
#include "be_visitor_union/union_ch.cpp"
#include "be_visitor_union/union_ci.cpp"
#include "be_visitor_union/union_cs.cpp"
#include "be_visitor_union/any_op_ch.cpp"
#include "be_visitor_union/any_op_cs.cpp"
#include "be_visitor_union/cdr_op_ch.cpp"
#include "be_visitor_union/cdr_op_ci.cpp"
#include "be_visitor_union/cdr_op_cs.cpp"

ACE_RCSID(be, be_visitor_union, "$Id$")
