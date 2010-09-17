
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for sequences. This uses
 *  compiled marshaling.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

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
      || node->imported ()
      || node->is_local ())
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
  if (tdef == 0)
    {
      *os << "\n\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_H_"
          << "\n#define _TAO_CDR_OP_" << node->flat_name () << "_H_";
    }

  bool alt = be_global->alt_mapping ();

  *os << be_global->core_versioning_begin ();

  *os << be_nl << be_nl
      << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const ";

  if (alt)
    {
      *os << "std::vector<" << base_type->name () << ">";
    }
  else
    {
      *os << node->name ();
    }

  *os << " &_tao_sequence" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl;

  if (alt)
    {
      *os << "std::vector<" << base_type->name () << ">";
    }
  else
    {
      *os << node->name ();
    }

  *os << " &_tao_sequence" << be_uidt_nl
      << ");" << be_uidt;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_nl << be_nl
          << be_global->stub_export_macro () << " std::ostream&"
          << " operator<< (" << be_idt_nl
          << "std::ostream &strm," << be_nl
          << "const " << node->name () << " &_tao_sequence);"
          << be_uidt;
    }

  *os << be_nl
      << be_global->core_versioning_end () << be_nl;

  if (tdef == 0)
    {
      *os << "\n\n#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_H_ */";
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
