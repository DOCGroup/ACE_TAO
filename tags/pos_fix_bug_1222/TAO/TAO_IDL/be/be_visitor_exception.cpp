//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_exception.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Exception
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_exception.h"

// include all the individual files
#include "be_visitor_exception/exception.cpp"
#include "be_visitor_exception/exception_ch.cpp"
#include "be_visitor_exception/exception_ci.cpp"
#include "be_visitor_exception/exception_cs.cpp"
#include "be_visitor_exception/exception_ctor.cpp"
#include "be_visitor_exception/ctor_assign.cpp"
#include "be_visitor_exception/any_op_ch.cpp"
#include "be_visitor_exception/any_op_cs.cpp"
#include "be_visitor_exception/cdr_op_ch.cpp"
#include "be_visitor_exception/cdr_op_ci.cpp"
#include "be_visitor_exception/cdr_op_cs.cpp"

ACE_RCSID(be, be_visitor_exception, "$Id$")
