
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  Visitor generating code for CDR operators for maps. This uses
 *  compiled marshaling.
 *
 *  @author Tyler Mayoff
 */
//=============================================================================

#include "map.h"

// ***************************************************************************
// Map visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_map_cdr_op_ch::be_visitor_map_cdr_op_ch (
    be_visitor_context *ctx) : be_visitor_decl (ctx)
{
}

int
be_visitor_map_cdr_op_ch::visit_map (be_map *node)
{
  if (node->cli_hdr_cdr_op_gen()
      || node->imported()
      || node->is_local())
    {
      return 0;
    }

  be_type *key_type = dynamic_cast<be_type*> (node->key_type ());

  // If our base type is an anonymous map, generate code for it here.
  if (key_type->node_type () == AST_Decl::NT_map)
    {
      if (key_type->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_map_cdr_op_ch::visit_map -"
                             "codegen for nested anonymous map failed\n"),
                            -1);
        }
    }

  be_type *value_type = dynamic_cast<be_type*> (node->value_type ());

  // If our base type is an anonymous map, generate code for it here.
  if (value_type->node_type () == AST_Decl::NT_map)
    {
      if (value_type->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_map_cdr_op_ch::visit_map -"
                             "codegen for nested anonymous map failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt = dynamic_cast<be_type*> (node);
  be_typedef *tdef = dynamic_cast<be_typedef*> (bt);

  TAO_INSERT_COMMENT(os);

  // If we're an anonymous map, we must protect against
  // being declared more than once.
  if (tdef == nullptr)
    {
      *os << "\n\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_H_"
          << "\n#define _TAO_CDR_OP_" << node->flat_name () << "_H_";
    }

  *os << be_global->core_versioning_begin();

  *os << be_nl_2
      << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name()
      << " &_tao_map);"
      << be_uidt << be_uidt_nl;

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      <<  node->name()
      << " &_tao_map);"
      << be_uidt << be_uidt;

  if (be_global->gen_ostream_operators())
    {
      node->gen_ostream_operator(os, false);
    }

  *os << be_nl << be_global->core_versioning_end() << be_nl;

  if (tdef == nullptr)
    {
      *os << "\n\n#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_H_ */";
    }

  node->cli_hdr_cdr_op_gen (true);

  return 0;
}
