//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_upcall_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for post-processing of return type after an
//    upcall is made
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_post_upcall_ss, "$Id$")


// ****************************************************************************
// visitor to do any post processing for return type after an upcall
// ****************************************************************************

be_visitor_operation_rettype_post_upcall_ss::be_visitor_operation_rettype_post_upcall_ss (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_post_upcall_ss::~be_visitor_operation_rettype_post_upcall_ss (void)
{
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_array (be_array *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_string (be_string * /* node*/)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_post_upcall_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_union (be_union *)
{
  return 0;
}
