//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_component_fwd.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for forward declared Component
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_component_fwd.h"
#include "be_helper.h"

#include "be_visitor_component_fwd.h"
#include "be_visitor_interface_fwd.h"
#include "be_visitor_context.h"

#include "be_visitor_component_fwd/component_fwd_ch.cpp"
#include "be_visitor_component_fwd/any_op_ch.cpp"
#include "be_visitor_component_fwd/cdr_op_ch.cpp"
#include "be_visitor_component_fwd/cdr_op_ci.cpp"

ACE_RCSID (be, 
           be_visitor_component_fwd, 
           "$Id$")

