//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_valuetype_fwd.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Valuetype_Fwd
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_extern.h"
#include "be_helper.h"

#include "be_visitor_valuetype_fwd.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"

#include "be_visitor_valuetype_fwd/valuetype_fwd_ch.cpp"
#include "be_visitor_valuetype_fwd/any_op_ch.cpp"
#include "be_visitor_valuetype_fwd/cdr_op_ch.cpp"
#include "be_visitor_valuetype_fwd/cdr_op_ci.cpp"

ACE_RCSID (be, 
           be_visitor_valuetype_fwd, 
           "$Id$")

