
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for unions. This uses
 *  compiled marshaling.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_cdr_op_ch::be_visitor_union_cdr_op_ch (
    be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_ch::~be_visitor_union_cdr_op_ch (void)
{
}

int
be_visitor_union_cdr_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO_OutputCDR &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO_InputCDR &, "
      << node->name () << " &);" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_global->stub_export_macro () << " std::ostream&"
          << " operator<< (std::ostream &strm, const "
          << node->name () << " &);" << be_nl;
    }

  *os << be_global->core_versioning_end () << be_nl;

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  be_visitor_context ctx (*this->ctx_);
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_localtypes);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      be_enum *e = be_enum::narrow_from_decl (d);
      if (e != 0)
        {
          be_visitor_enum_cdr_op_ch visitor (&ctx);

          if (e->accept (&visitor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%N:%l) be_visitor_union_cdr_op_ch::visit_union"
                          " - codegen for enum failed\n"));
            }

          // Restore the union node in the enum visitor's context.
          ctx.node (this->ctx_->node ());
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ch::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
