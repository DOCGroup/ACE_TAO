//
// $Id$
//
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    amh_rh_sh.cpp
//
// = DESCRIPTION
//    Visitor generating AMH-RH skeleton code for Operation node in the
//    skeleton header.
//
// = AUTHOR
//    Mayur Deshpande <mayur@ics.uci.edu>
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID (be_visitor_operation, 
           operation_amh_rh_sh, 
           "$Id$")


// ******************************************************
// Visitor for generating AMH-RH skeleton for "operation"
// in skeleton header.
// ******************************************************

be_visitor_amh_rh_operation_sh::be_visitor_amh_rh_operation_sh (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_sh::~be_visitor_amh_rh_operation_sh (void)
{
}

int
be_visitor_amh_rh_operation_sh::visit_operation (be_operation *node)
{
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_rh_operation_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Step 1 : Generate return type: always void
  os->indent ();
  *os << "virtual void ";

  // Step 2: Generate the method name
  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "set_";
        }
      else
        {
          *os << "get_";
        }
    }

  *os << node->local_name();

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  be_visitor_operation_arglist visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_amh_rh_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << ";" << be_nl;

  return 0;
}
