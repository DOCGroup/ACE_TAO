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
//    Visitors for generation of code for Enum for the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_enum.h"

ACE_RCSID(be_visitor_enum, cdr_op_cs, "$Id$")


// ***************************************************************************
// Enum visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_enum_cdr_op_cs::be_visitor_enum_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_enum_cdr_op_cs::~be_visitor_enum_cdr_op_cs (void)
{
}

int
be_visitor_enum_cdr_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();


  // generate CDR << and >> operators
  os->indent ();
  *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "return strm.write_ulong ((CORBA::ULong) _tao_enumval);" << be_uidt_nl
      << "}\n\n";

  *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << " &_tao_enumval)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::ULong _tao_temp;" << be_nl
      << "if (strm.read_ulong (_tao_temp))" << be_nl
      << "{" << be_idt_nl
      << "ACE_OS::memcpy (&_tao_enumval, &_tao_temp, sizeof (CORBA::ULong));" 
      << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt << be_uidt_nl
      << "}\n\n";

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
