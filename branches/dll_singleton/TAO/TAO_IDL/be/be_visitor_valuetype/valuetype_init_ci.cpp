//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes factory in the client header
//    (see IDL to C++ mapping)
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, valuetype_init_ci, "$Id$")

be_visitor_valuetype_init_ci::be_visitor_valuetype_init_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_ci::~be_visitor_valuetype_init_ci (void)
{
}

int
be_visitor_valuetype_init_ci::visit_valuetype (be_valuetype *)
{
  return 0;
}

int
be_visitor_valuetype_init_ci::visit_factory (be_factory *)
{
  return 0;
}
