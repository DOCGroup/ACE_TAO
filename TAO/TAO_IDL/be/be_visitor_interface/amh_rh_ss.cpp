//=============================================================================
/**
*  @file   amh_rh_ss.cpp
*
*  Specialized interface visitor for AMH-ResponseHandler that generates code for
*  RH in the skeleton source files.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#include "interface.h"

be_visitor_amh_rh_interface_ss::be_visitor_amh_rh_interface_ss (
    be_visitor_context *ctx)
  : be_visitor_interface_ss (ctx)
{
}

int
be_visitor_amh_rh_interface_ss::visit_operation (be_operation *node)
{
  be_visitor_amh_rh_operation_ss amh_rh_op (this->ctx_);
  return amh_rh_op.visit_operation (node);
}

int
be_visitor_amh_rh_interface_ss::visit_interface (be_interface *node)
{
  if (node->srv_skel_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Generate code for elements in the scope (e.g., operations)
  // We'll rely on the base class (be_visitor_scope) to do the
  // right thing for us.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}
