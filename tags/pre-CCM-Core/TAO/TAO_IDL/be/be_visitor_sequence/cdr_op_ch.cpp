//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for sequences. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, cdr_op_ch, "$Id$")

// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_cdr_op_ch::be_visitor_sequence_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_ch::~be_visitor_sequence_cdr_op_ch (void)
{
}

int
be_visitor_sequence_cdr_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_cdr_op_gen () ||
      node->imported () ||
      node->is_local ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations

  be_type *bt = be_type::narrow_from_decl (node);
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (!tdef)
    {
      *os << "\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_H_" << be_nl
          << "#define _TAO_CDR_OP_" << node->flat_name () << "_H_\n\n";
    }

  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt << "\n\n";

  if (!tdef)
    {
      *os << "#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_H_ */\n\n";
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
