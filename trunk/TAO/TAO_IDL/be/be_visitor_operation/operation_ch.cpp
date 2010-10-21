
//=============================================================================
/**
 *  @file    operation_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation node in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Primary visitor for "operation" in client header.
// ******************************************************

be_visitor_operation_ch::be_visitor_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ch::~be_visitor_operation_ch (void)
{
}

int
be_visitor_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  // Every operation is declared virtual in the client code.
  *os << be_uidt << be_uidt_nl << be_idt << be_idt_nl
      << "virtual ";

  // STEP I: generate the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_ch::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("Bad return type\n")),
                        -1);
    }

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
    be_interface::narrow_from_scope (node->defined_in ());

  /// If we are in a reply handler, are not an execp_* operation,
  /// and have no native args, then generate the AMI static
  /// reply stub declaration.
  if (intf != 0
      && intf->is_ami_rh ()
      && !node->is_excep_ami ()
      && !node->has_native ())
    {
      *os << be_nl << be_nl
          << "static void " << be_nl
          << node->local_name () << "_reply_stub (" << be_idt_nl
          << "TAO_InputCDR &_tao_reply_cdr," << be_nl
          << "::Messaging::ReplyHandler_ptr _tao_reply_handler,"
          << be_nl
          << "::CORBA::ULong reply_status);" << be_uidt;
    }

  return 0;
}
