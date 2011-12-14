//=============================================================================
/**
 *  @file    amh_rh_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating AMH-RH skeleton code for Operation node in the
 *  skeleton header.
 *
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
//=============================================================================

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

  /// These are not for the server side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  UTL_Scope *s =
    this->ctx_->attribute ()
      ? this->ctx_->attribute ()->defined_in ()
      : node->defined_in ();

  // Needs to be one or the other.
  if (be_interface::narrow_from_scope (s) == 0
      && be_porttype::narrow_from_scope (s) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_amh_rh_operation_sh::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("bad scope\n")),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

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

  *os << this->ctx_->port_prefix ().c_str ()
      << node->local_name();

  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_arglist visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_amh_rh_sh::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("codegen for argument list failed\n")),
                        -1);
    }

  *os << ";";

  return 0;
}
