//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    marshal_ch.cpp
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for marshalling.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, marshal_ch, "$Id$")

// ***************************************************************************
// Structure visitor for generating  declarations
// ***************************************************************************

be_visitor_valuetype_marshal_ch::be_visitor_valuetype_marshal_ch
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_marshal_ch::~be_visitor_valuetype_marshal_ch (void)
{
}

int
be_visitor_valuetype_marshal_ch::visit_valuetype (be_valuetype *node)
{
   return 0;
}
