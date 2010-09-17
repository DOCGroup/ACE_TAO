
//=============================================================================
/**
 *  @file    serializer_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TAO::DCPS::Serializer operators for sequences.
 *  This uses compiled marshaling.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Sequence visitor for generating Serializer operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_serializer_op_ch::be_visitor_sequence_serializer_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_serializer_op_ch::~be_visitor_sequence_serializer_op_ch (void)
{
}

int
be_visitor_sequence_serializer_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_serializer_op_gen ()
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
                             "be_visitor_sequence_serializer_op_ch::visit_sequence -"
                             "codegen for nested anonymous sequence failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt = be_type::narrow_from_decl (node);
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  if (be_global->gen_dcps_type_support_only ())
    {
      *os << "\n#endif /* end of disabling TAO specific code */"
          << be_nl << be_nl;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (!tdef)
    {
      *os << "\n\n#if !defined _TAO_SERIALIZER_OP_"
          << node->flat_name () << "_H_"
          << "\n#define _TAO_SERIALIZER_OP_" << node->flat_name () << "_H_";
    }

  *os << be_nl << be_nl;

  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean _tao_is_bounded_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_max_marshaled_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_find_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_nl << be_nl
      << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt;

  if (!tdef)
    {
      *os << "\n\n#endif /* _TAO_SERIALIZER_OP_"
          << node->flat_name () << "_H_ */";
    }

  if (be_global->gen_dcps_type_support_only ())
    {
      *os << be_nl  << be_nl << "#if 0 // disable TAO specific code"
          << be_nl << be_nl;
    }
  node->cli_hdr_serializer_op_gen (1);
  return 0;
}
