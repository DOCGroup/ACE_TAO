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
be_visitor_valuetype_ami_exception_holder_ch::visit_valuetype (
    be_valuetype *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the implemenation of the Messaging aware ORB.
  *os << be_nl << be_nl
      << "class _tao_" << node->local_name () << be_idt_nl
      << ": public ";

  if (!node->is_nested ())
    {
      *os << "OBV_";
    }

  *os << node->local_name () << "," << be_nl
      << "  public virtual OBV_Messaging::ExceptionHolder," << be_nl
      << "  public virtual ::CORBA::DefaultValueRefCountBase" << be_uidt_nl
      << "{" << be_nl;
  *os << "public:" << be_idt_nl;
  *os << "_tao_" << node->local_name () << " ();" << be_nl << be_nl;
  *os << "~_tao_" << node->local_name () << " ();";

  if (this->visit_valuetype_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_"
                         "exception_holder_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
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
