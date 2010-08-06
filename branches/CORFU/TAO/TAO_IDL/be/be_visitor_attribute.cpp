//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_attribute.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Attribute
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_attribute.h"
#include "be_operation.h"
#include "be_predefined_type.h"
#include "be_argument.h"

#include "be_visitor_attribute.h"
#include "be_visitor_context.h"
#include "be_visitor_operation.h"

#include "ast_generator.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

#include "be_visitor_attribute/attribute.cpp"

ACE_RCSID (be, 
           be_visitor_attribute, 
           "$Id$")

