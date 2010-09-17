
//=============================================================================
/**
 *  @file    serializer_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for Enum for the
 *  TAO::DCPS::Serializer operators
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Enum visitor for generating Serializer operator declarations in the client
// stubs file.
// ***************************************************************************

be_visitor_enum_serializer_op_cs::be_visitor_enum_serializer_op_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_enum_serializer_op_cs::~be_visitor_enum_serializer_op_cs (void)
{
}

int
be_visitor_enum_serializer_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_serializer_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "::CORBA::Boolean operator<< (TAO::DCPS::Serializer &strm, "
      << "const " << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::ULong _tao_temp = _tao_enumval;" << be_nl
      << "return strm << _tao_temp;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "::CORBA::Boolean operator>> (TAO::DCPS::Serializer &strm, "
      << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::ULong _tao_temp = 0;" << be_nl
      << "::CORBA::Boolean _tao_result = strm >> _tao_temp;" << be_nl << be_nl
      << "if (_tao_result == 1)" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_enumval = static_cast<" << node->name ()
      << "> (_tao_temp);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return _tao_result;" << be_uidt_nl
      << "}";

  node->cli_stub_serializer_op_gen (true);
  return 0;
}
