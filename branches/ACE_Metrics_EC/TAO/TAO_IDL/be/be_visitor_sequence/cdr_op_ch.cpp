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

ACE_RCSID (be_visitor_sequence, 
           cdr_op_ch, 
           "$Id$")

// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_cdr_op_ch::be_visitor_sequence_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_ch::~be_visitor_sequence_cdr_op_ch (void)
{
}

int
be_visitor_sequence_cdr_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_type *base_type = be_type::narrow_from_decl (node->base_type ());

  // If our base type is an anonymous sequence, generate code for it here.
  if (base_type->node_type () == AST_Decl::NT_sequence)
    {
      if (base_type->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_ch::visit_sequence -"
                             "codegen for nested anonymous sequence failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt = be_type::narrow_from_decl (node);
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (!tdef)
    {
      *os << "\n\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_H_"
          << "\n#define _TAO_CDR_OP_" << node->flat_name () << "_H_";
    }

  *os << be_nl << be_nl 
      << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt;

  if (!tdef)
    {
      *os << "\n\n#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_H_ */";
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
