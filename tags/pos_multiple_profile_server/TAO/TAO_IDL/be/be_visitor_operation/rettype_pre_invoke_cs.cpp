//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_pre_invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for doing any pre-processing of return type prior
//    to making the do_static_call.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_pre_invoke_cs, "$Id$")


// *****************************************************************************
//    be_visitor_operation_rettype_pre_invoke_cs
//
// *****************************************************************************

be_visitor_operation_rettype_pre_invoke_cs::
be_visitor_operation_rettype_pre_invoke_cs (be_visitor_context *ctx)
  : be_visitor_operation_rettype_pre_docall_cs (ctx)
{
}

be_visitor_operation_rettype_pre_invoke_cs::
~be_visitor_operation_rettype_pre_invoke_cs (void)
{
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_interface (be_interface *)
{
  // don't do anything. This is the overriding action
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

