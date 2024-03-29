
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  Visitors for generation of code for Enum for the CDR operators
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "enum.h"

be_visitor_enum_cdr_op_cs::be_visitor_enum_cdr_op_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

int
be_visitor_enum_cdr_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->core_versioning_begin () << be_nl;

  *os << "::CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << node->name () << " _tao_enumerator)" << be_nl
      << "{" << be_idt_nl
      << "return strm << static_cast< ::CORBA::ULong> (_tao_enumerator);"
      << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << " & _tao_enumerator)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::ULong _tao_temp = 0;" << be_nl
      << "::CORBA::Boolean const _tao_success = strm >> _tao_temp;" << be_nl
      << be_nl
      << "if (_tao_success)" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_enumerator = static_cast<" << node->name ()
      << "> (_tao_temp);" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return _tao_success;" << be_uidt_nl
      << "}" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (true);
  return 0;
}
