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
//    Visitor for code generation of Sequences for the CDR operators
//    in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, cdr_op_ci, "$Id$")


// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_sequence_cdr_op_ci::be_visitor_sequence_cdr_op_ci
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_ci::~be_visitor_sequence_cdr_op_ci (void)
{
}

int
be_visitor_sequence_cdr_op_ci::visit_sequence (be_sequence *node)
{
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // Sequences are *always* generated in the .cpp file, it doesn't
  // save all that time to generate them inline and this breaks
  // the dependencies for recursive types.

  os->indent ();
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt << "\n\n";

  node->cli_inline_cdr_op_gen (1);

  return 0;
}
