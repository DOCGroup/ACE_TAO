//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           cdr_op_cs, 
           "$Id$")

be_visitor_interface_cdr_op_cs::be_visitor_interface_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_cs::~be_visitor_interface_cdr_op_cs (void)
{
}

int
be_visitor_interface_cdr_op_cs::visit_interface (be_interface *node)
{
  // No CDR operations for local interfaces.
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  // Visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cdr_op_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->full_name () << "_ptr _tao_objref" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "CORBA::Object_ptr _tao_corba_obj = _tao_objref;" << be_nl;
  *os << "return (strm << _tao_corba_obj);" << be_uidt_nl
      << "}\n\n";

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->full_name () << "_ptr &_tao_objref" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;
  *os << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl;
  *os << "CORBA::Object_var obj;" << be_nl;
  *os << "if ((strm >> obj.inout ()) == 0)" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "// narrow to the right type" << be_nl;
  *os << "_tao_objref =" << be_idt_nl
      << node->full_name () << "::_unchecked_narrow ("
      << be_idt << be_idt_nl
      << "obj.in ()" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl;
  *os << "ACE_TRY_CHECK;" << be_nl;
  *os << "return 1;" << be_uidt_nl;
  *os << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "// do nothing" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl;
  *os << "}\n\n";

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
