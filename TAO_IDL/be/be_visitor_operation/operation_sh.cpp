
//=============================================================================
/**
 *  @file    operation_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// Operation visitor for server header
// ************************************************************

be_visitor_operation_sh::be_visitor_operation_sh (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_sh::~be_visitor_operation_sh (void)
{
}

int
be_visitor_operation_sh::visit_operation (be_operation *node)
{
  /// No server-side code generation for these implied IDL nodes.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "virtual ";

  // STEP I: generate the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_SH);
  be_visitor_operation_arglist oa_visitor (&ctx);

  if (node->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Generate the corresponding static skeleton method for this operation only
  // if there was no "native" type.
  if (!node->has_native ())
    {
      *os << be_nl_2
          << "static void ";

      // Check if we are an attribute node in disguise.
      if (this->ctx_->attribute ())
        {
          // Now check if we are a "get" or "set" operation
          if (node->nmembers () == 1)
            {
              *os << "_set_";
            }
          else
            {
              *os << "_get_";
            }
        }

      *os << node->local_name ()
          << "_skel (" << be_idt << be_idt_nl
          << "TAO_ServerRequest & server_request," << be_nl
          << "void * servant_upcall," << be_nl
          << "void * servant);" << be_uidt
          << be_uidt;
    }

  return 0;
}
