//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface_fwd.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Interface_Fwd
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_interface_fwd.h"
#include "be_interface.h"
#include "be_helper.h"
#include "be_extern.h"

#include "be_visitor_interface_fwd.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"

#include "be_visitor_interface_fwd/interface_fwd_ch.cpp"
#include "be_visitor_interface_fwd/cdr_op_ch.cpp"
#include "be_visitor_interface_fwd/cdr_op_ci.cpp"
#include "be_visitor_interface_fwd/any_op_ch.cpp"

ACE_RCSID (be, 
           be_visitor_interface_fwd, 
           "$Id$")
