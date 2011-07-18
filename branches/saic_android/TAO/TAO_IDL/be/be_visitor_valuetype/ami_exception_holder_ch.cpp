//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_ch.cpp
//
// = DESCRIPTION
//    Visitor generating exception holders for the AMI callback model.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype,
           ami_exception_holder_ch,
           "$Id$")

// ******************************************************
// Interface visitor for client header.
// ******************************************************

be_visitor_valuetype_ami_exception_holder_ch::
be_visitor_valuetype_ami_exception_holder_ch (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ami_exception_holder_ch::
~be_visitor_valuetype_ami_exception_holder_ch (void)
{
}

int
be_visitor_valuetype_ami_exception_holder_ch::visit_operation (
    be_operation *node
  )
{
  be_visitor_context ctx (*this->ctx_);

  // Using the implementation class visitor is strange, but we
  // do it here because it's the only one that generates the
  // environment variable in the operation signature without
  // the trailing _WITH_DEFAULTS, which is what we want.
  // For performance reasons, we would rather there be a
  // compile error if the user does not pass an environment
  // variable, than create a default one, which causes extra
  // TSS activity.
  be_visitor_operation_ih visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_"
                         "ami_exception_holder_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt;

  return 0;
}
