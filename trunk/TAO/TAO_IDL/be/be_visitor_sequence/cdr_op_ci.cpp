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

ACE_RCSID (be_visitor_sequence, 
           cdr_op_ci, 
           "$Id$")

// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_sequence_cdr_op_ci::be_visitor_sequence_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_ci::~be_visitor_sequence_cdr_op_ci (void)
{
}

int
be_visitor_sequence_cdr_op_ci::visit_sequence (be_sequence *node)
{
  if (node->cli_inline_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // If we contain an anonymous sequence,
  // generate code for the sequence here.

  // Retrieve the base type.
  be_type *base = be_type::narrow_from_decl(node->base_type ());
  AST_Decl::NodeType nt = base->node_type ();

  // If our base type is an anonymous sequence, generate code for it here.
  if (nt == AST_Decl::NT_sequence && base->anonymous ())
    {
      if (base->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_sequence_cdr_op_ci::"
                             "visit_sequence - "
                             "accept on anonymous base type failed\n"),
                            -1);
        }
    }

  // Sequence CDR operators are always generated in the .cpp file, it doesn't
  // save all that much time to generate them inline and this breaks
  // the dependencies for recursive types.

  be_type *bt = be_type::narrow_from_decl (node);
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (!tdef)
    {
      *os << "\n\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_I_" << be_nl
          << "#define _TAO_CDR_OP_" << node->flat_name () << "_I_";

    }

  *os << be_nl << be_nl
      << "CORBA::Boolean " << be_global->stub_export_macro ()
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;
  *os << "CORBA::Boolean " << be_global->stub_export_macro ()
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt;

  if (!tdef)
    {
      *os << "\n\n" << "#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_I_ */";
    }

  node->cli_inline_cdr_op_gen (1);

  return 0;
}
