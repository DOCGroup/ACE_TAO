//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_eventtype_fwd.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Eventtype_Fwd
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_eventtype_fwd.h"

#include "be_visitor_eventtype_fwd.h"
#include "be_visitor_valuetype_fwd.h"
#include "be_visitor_context.h"

#include "be_visitor_eventtype_fwd/eventtype_fwd_ch.cpp"
#include "be_visitor_eventtype_fwd/any_op_ch.cpp"
#include "be_visitor_eventtype_fwd/cdr_op_ch.cpp"
#include "be_visitor_eventtype_fwd/cdr_op_ci.cpp"

ACE_RCSID (be, 
           be_visitor_eventtype_fwd, 
           "$Id$")

