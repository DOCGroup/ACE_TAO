//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init.cpp
//
// = DESCRIPTION
//    Coomon visitor for valuetypes factory construct
//    (see IDL to C++ mapping). Based on ptc/00-01-02.
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

ACE_RCSID(be_visitor_valuetype, valuetype_init, "$Id$")

be_visitor_valuetype_init::be_visitor_valuetype_init (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_init::~be_visitor_valuetype_init (void)
{
}

int
be_visitor_valuetype_init::visit_attribute (be_attribute *)
{
  return 0;
}

int 
be_visitor_valuetype_init::visit_constant (be_constant *)
{
  return 0;
}

int 
be_visitor_valuetype_init::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_valuetype_init::visit_operation (be_operation *)
{
  return 0;
}

int
be_visitor_valuetype_init::visit_exception (be_exception *)
{
  return 0;
}

int 
be_visitor_valuetype_init::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_valuetype_init::visit_union (be_union *)
{
  return 0;
}

int 
be_visitor_valuetype_init::visit_typedef (be_typedef *)
{
  return 0;
}

int
be_visitor_valuetype_init::visit_field (be_field *)
{
  return 0;
}

int 
be_visitor_valuetype_init::visit_factory (be_factory *)
{
  return 0;
}

void
be_visitor_valuetype_init::begin_public (void)
{
}

void
be_visitor_valuetype_init::begin_private (void)
{
}
