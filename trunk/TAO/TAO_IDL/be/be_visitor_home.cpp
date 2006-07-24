//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_home.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for a component home.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_home.h"

#include "be_visitor_home.h"
#include "be_visitor_typecode/typecode_defn.h"
#include "be_visitor_typecode/objref_typecode.h"
#include "be_visitor_context.h"
#include "be_helper.h"
#include "be_extern.h"
#include "ace/Log_Msg.h"

#include "be_visitor_home/home_ch.cpp"
#include "be_visitor_home/home_cs.cpp"

ACE_RCSID (be, 
           be_visitor_home, 
           "$Id$")

