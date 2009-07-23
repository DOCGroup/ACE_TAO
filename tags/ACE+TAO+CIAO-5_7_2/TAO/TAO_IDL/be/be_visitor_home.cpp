//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
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

#include "ace/Log_Msg.h"

#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "ast_argument.h"
#include "ast_exception.h"
#include "nr_extern.h"

#include "be_helper.h"
#include "be_identifier_helper.h"
#include "be_extern.h"
#include "be_argument.h"
#include "be_attribute.h"
#include "be_home.h"
#include "be_component.h"
#include "be_operation.h"
#include "be_sequence.h"
#include "be_string.h"

#include "be_visitor_home.h"
#include "be_visitor_attribute.h"
#include "be_visitor_operation.h"
#include "be_visitor_typecode/typecode_defn.h"
#include "be_visitor_typecode/objref_typecode.h"
#include "be_visitor_context.h"

#include "be_visitor_home/home_ch.cpp"
#include "be_visitor_home/home_cs.cpp"
#include "be_visitor_home/home_svh.cpp"
#include "be_visitor_home/home_svs.cpp"
#include "be_visitor_home/home_ex_idl.cpp"
#include "be_visitor_home/home_exh.cpp"
#include "be_visitor_home/home_exs.cpp"

ACE_RCSID (be, 
           be_visitor_home, 
           "$Id$")

