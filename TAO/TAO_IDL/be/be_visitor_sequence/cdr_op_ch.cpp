
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  Visitor generating code for CDR operators for sequences. This uses
 *  compiled marshaling.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "sequence.h"

// ***************************************************************************
// Sequence visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_cdr_op_ch::be_visitor_sequence_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cdr_op_ch::~be_visitor_sequence_cdr_op_ch () = default;

int
be_visitor_sequence_cdr_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  if (idl_global->dcps_sequence_type_defined (node->full_name ()))
    {
      return 0;
    }

  be_type * const base_type = dynamic_cast<be_type*> (node->base_type ());

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

  TAO_OutStream * const os = this->ctx_->stream ();

  be_type * const bt = dynamic_cast<be_type*> (node);
  be_typedef * const tdef = dynamic_cast<be_typedef*> (bt);

  TAO_INSERT_COMMENT (os);

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (tdef == nullptr)
    {
      *os << "\n\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_H_"
          << "\n#define _TAO_CDR_OP_" << node->flat_name () << "_H_";
    }

  bool const alt = be_global->alt_mapping ();
  std::string sequence_type;

  if (alt)
    {
      sequence_type = "std::vector<" + base_type->full_name () + ">";
    }
  else
    {
      sequence_type = node->full_name ();
    }

  *os << be_global->core_versioning_begin ();

  std::string declaration = be_global->stub_export_macro ();
  declaration += " ::CORBA::Boolean operator<< (TAO_OutputCDR &strm, ";
  declaration += sequence_type;
  declaration += " const &_tao_sequence);";

  *os << be_nl_2;
  if (declaration.size () <= 120)
    {
      *os << declaration;
    }
  else
    {
      *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
          << " operator<< (" << be_idt << be_nl
          << "TAO_OutputCDR &strm," << be_nl
          << sequence_type << " const &_tao_sequence);" << be_uidt;
    }

  *os << be_nl;

  declaration = be_global->stub_export_macro ();
  declaration += " ::CORBA::Boolean operator>> (TAO_InputCDR &strm, ";
  declaration += sequence_type;
  declaration += " &_tao_sequence);";

  if (declaration.size () <= 120)
    {
      *os << declaration;
    }
  else
    {
      *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
          << " operator>> (" << be_idt << be_nl
          << "TAO_InputCDR &strm," << be_nl
          << sequence_type << " &_tao_sequence);" << be_uidt;
    }

  *os << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      declaration = be_global->stub_export_macro ();
      declaration += " std::ostream &operator<< (std::ostream &strm, ";
      declaration += node->full_name ();
      declaration += " const &_tao_sequence);";

      *os << be_nl_2;
      if (declaration.size () <= 120)
        {
          *os << declaration;
        }
      else
        {
          *os << be_global->stub_export_macro () << " std::ostream &operator<< ("
              << be_idt << be_nl << "std::ostream &strm," << be_nl
              << node->name () << " const &_tao_sequence);" << be_uidt;
        }
    }

  *os << be_nl
      << be_global->core_versioning_end () << be_nl;

  if (tdef == nullptr)
    {
      *os << "\n\n#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_H_ */";
    }

  node->cli_hdr_cdr_op_gen (true);
  return 0;
}
