//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_constant.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Constant
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_constant.h"
#include "be_module.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"

#include "be_visitor_constant.h"
#include "be_visitor_context.h"

#include "be_visitor_constant/constant_ch.cpp"
#include "be_visitor_constant/constant_cs.cpp"

ACE_RCSID (be, 
           be_visitor_constant, 
           "$Id$")

