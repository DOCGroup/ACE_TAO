//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Enum for the Any operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_enum.h"

ACE_RCSID(be_visitor_enum, any_op_cs, "$Id$")


// ***************************************************************************
// Enum visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_enum_any_op_cs::be_visitor_enum_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_enum_any_op_cs::~be_visitor_enum_any_op_cs (void)
{
}

int
be_visitor_enum_any_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " _tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "TAO_OutputCDR stream;" << be_nl
      << "stream << _tao_elem;" << be_nl
      << "_tao_any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl
      << "TAO_ENCAP_BYTE_ORDER," << be_nl
      << "stream.begin ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << " &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl << be_nl
      << "CORBA::Boolean result = type->equivalent (" << node->tc_name ()
      << " ACE_ENV_ARG_PARAMETER);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl << be_nl
      << "if (!result)" << be_idt_nl
      << "return 0; // not equivalent" << be_uidt_nl << be_nl
      << "TAO_InputCDR stream (" << be_idt << be_idt_nl
      << "_tao_any._tao_get_cdr ()," << be_nl
      << "_tao_any._tao_byte_order ()" << be_uidt_nl
      << ");" << be_uidt_nl
      << "if (stream >> _tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}\n\n";

  node->cli_stub_any_op_gen (1);
  return 0;
}
