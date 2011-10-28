
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valueboxes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


be_visitor_valuebox_cdr_op_cs::be_visitor_valuebox_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_cdr_op_cs::~be_visitor_valuebox_cdr_op_cs (void)
{
}

int
be_visitor_valuebox_cdr_op_cs::visit_valuebox (be_valuebox *node)
{
  // Already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || ! node->is_defined ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  node->cli_stub_cdr_op_gen (true);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

*os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean" << be_nl
      << "operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->full_name ()
      << " *_tao_valuebox" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "return" << be_idt_nl
      << "::CORBA::ValueBase::_tao_marshal (" << be_idt << be_idt_nl
      << "strm," << be_nl
      << "_tao_valuebox," << be_nl
      << "reinterpret_cast<ptrdiff_t> (&"
      << node->full_name () << "::_downcast)"
      << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean" << be_nl
      << "operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->full_name ()
      << " *&_tao_valuebox" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "return " << node->full_name ()
      << "::_tao_unmarshal (strm, _tao_valuebox);"
      << be_uidt_nl
      << "}" << be_nl_2;

  *os << be_global->core_versioning_end () << be_nl;

  return 0;
}
