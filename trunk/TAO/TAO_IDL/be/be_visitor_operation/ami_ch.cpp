//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_ch.cpp
//
// = DESCRIPTION
//    Visitor generating AMI stub code for Operation node in the
//    client header.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           operation_ami_ch, 
           "$Id$")

// ******************************************************
// Visitor for generating AMI stub for "operation" in client header.
// ******************************************************

be_visitor_operation_ami_ch::be_visitor_operation_ami_ch (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_ch::~be_visitor_operation_ami_ch (void)
{
}

int
be_visitor_operation_ami_ch::visit_operation (be_operation *node)
{
  // No sendc method for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Every operation is declared virtual in the client code.
  *os << be_nl << be_nl
      << "virtual ";

  // STEP I: Return type is void.
  *os << "void ";

  // STEP 2: Generate the operation name.

  // First the sendc prefix.
  *os << "sendc_";

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

  *os << node->local_name ();

  // STEP 3: Generate the argument list with the appropriate
  //         mapping. For these we grab a visitor that generates the
  //         parameter listing.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  be_visitor_operation_arglist visitor (&ctx);

  if (node->arguments ()->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return 0;
}
