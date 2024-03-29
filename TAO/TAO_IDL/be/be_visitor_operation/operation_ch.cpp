
//=============================================================================
/**
 *  @file    operation_ch.cpp
 *
 *  Visitor generating code for Operation node in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "operation.h"

// ******************************************************
// Primary visitor for "operation" in client header.
// ******************************************************

be_visitor_operation_ch::be_visitor_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ch::~be_visitor_operation_ch ()
{
}

int
be_visitor_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  *os << be_nl_2;

  // STEP I: generate the return type.
  be_type *bt = dynamic_cast<be_type*> (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_ch::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("Bad return type\n")),
                        -1);
    }
  //Only if we are generating exec header file, generate DOxygen documentation
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_EXH)
    {
      if (this->void_return_type (bt))
        {
           *os << "/// Setter for " << node->local_name() << " attribute" << be_nl
               << "/// @param[in] " << node->local_name() << " - New value for "
               << node->local_name() << " attribute" << be_nl;
        }
      else
        {
          *os << "/// Getter for " << node->local_name() << " attribute" << be_nl
                 << "/// @return value of " << node->local_name() << " attribute" << be_nl;
        }
    }
  *os << "virtual ";

  // Grab the right visitor to generate the return type.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype or_visitor (&ctx);

  if (bt->accept (&or_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // STEP 2: generate the operation name. The port prefix should
  // be an empty string except for operations from attributes
  // defined in a porttype.
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  be_visitor_operation_arglist oa_visitor (&ctx);

  if (node->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  be_interface *intf =
    dynamic_cast<be_interface*> (node->defined_in ());

  /// If we are in a reply handler, are not an excep_* operation,
  /// and have no native args, then generate the AMI static
  /// reply stub declaration.
  if (intf != nullptr
      && intf->is_ami_rh ()
      && !node->is_excep_ami ()
      && !node->has_native ())
    {
      *os << be_nl_2
          << "static void" << be_nl
          << node->local_name () << "_reply_stub (" << be_idt_nl
          << "TAO_InputCDR &_tao_reply_cdr," << be_nl
          << "::Messaging::ReplyHandler_ptr _tao_reply_handler,"
          << be_nl
          << "::CORBA::ULong reply_status);" << be_uidt;
    }

  return 0;
}
