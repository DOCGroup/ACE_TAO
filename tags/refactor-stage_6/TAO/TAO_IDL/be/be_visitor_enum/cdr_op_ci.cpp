//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Enum for the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_enum, 
           cdr_op_ci, 
           "$Id$")


// ***************************************************************************
// Enum visitor for generating CDR operator declarations in the client
// stubs file.
// ***************************************************************************

be_visitor_enum_cdr_op_ci::be_visitor_enum_cdr_op_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_enum_cdr_op_ci::~be_visitor_enum_cdr_op_ci (void)
{
}

int
be_visitor_enum_cdr_op_ci::visit_enum (be_enum *node)
{
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::ULong _tao_temp = _tao_enumval;" << be_nl
      << "return strm << _tao_temp;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::ULong _tao_temp = 0;" << be_nl
      << "CORBA::Boolean _tao_result = strm >> _tao_temp;" << be_nl << be_nl
      << "if (_tao_result == 1)" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_enumval = ACE_static_cast (" << node->name ()
      << ", _tao_temp);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return _tao_result;" << be_uidt_nl
      << "}";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}
