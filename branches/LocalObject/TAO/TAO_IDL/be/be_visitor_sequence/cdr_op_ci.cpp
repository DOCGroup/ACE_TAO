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
  if (node->cli_inline_cdr_op_gen () ||
      node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // If we contain an anonymous sequence,
  // generate code for the sequence here.

  // retrieve the base type
  be_type *base = be_type::narrow_from_decl (node->base_type ());
  if (!base)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cdr_op_ci::"
                         "visit_sequence - "
                         "Bad base type\n"),
                        -1);
    }

  if (base->node_type () == AST_Decl::NT_sequence)
    {
      // CDR operators for sequences are now declared in the .i file,
      // so we pass this state to the function.
      if (this->gen_anonymous_base_type (base,
                                         TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CI)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cdr_op_ci::"
                             "visit_sequence - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // Sequences are *always* generated in the .cpp file, it doesn't
  // save all that time to generate them inline and this breaks
  // the dependencies for recursive types.

  be_type *bt = be_type::narrow_from_decl (node);
  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  // If we're an anonymous sequence, we must protect against
  // being declared more than once.
  if (!tdef)
    {
      *os << "\n#if !defined _TAO_CDR_OP_"
          << node->flat_name () << "_I_" << be_nl
          << "#define _TAO_CDR_OP_" << node->flat_name () << "_I_\n\n";
    }

  *os << "CORBA::Boolean " << idl_global->stub_export_macro ()
      << " operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << "CORBA::Boolean " << idl_global->stub_export_macro ()
      << " operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << " &" << be_uidt_nl
      << ");" << be_uidt << "\n\n";

  if (!tdef)
    {
      *os << "#endif /* _TAO_CDR_OP_"
          << node->flat_name () << "_I_ */\n\n";
    }

  node->cli_inline_cdr_op_gen (1);

  return 0;
}
