
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  Visitor generating code for CDR operators for unions
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_cdr_op_cs::be_visitor_union_cdr_op_cs (
    be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_cs::~be_visitor_union_cdr_op_cs ()
{
}

namespace {
  void
  serialize_disc (TAO_OutStream *os, AST_Expression::ExprType disc_type, bool out)
  {
    const char* tmp_suffix = nullptr;
    switch (disc_type)
      {
        case AST_Expression::EV_bool:
          tmp_suffix = "boolean";
          break;
        case AST_Expression::EV_char:
          tmp_suffix = "char";
          break;
        case AST_Expression::EV_wchar:
          tmp_suffix = "wchar";
          break;
        case AST_Expression::EV_uint8:
          tmp_suffix = "uint8";
          break;
        case AST_Expression::EV_int8:
          tmp_suffix = "int8";
          break;
        default:
          break;
      }

    const char* val = out ? "_tao_union._d ()" : "_tao_discriminant";
    if (tmp_suffix)
      {
        *os << (out ? "::ACE_OutputCDR::from_" : "::ACE_InputCDR::to_")
          << tmp_suffix << " tmp (" << val << ");" << be_nl;
        val = "tmp";
      }

    *os
      << "if (!(strm " << (out ? "<<" : ">>") << " " << val << "))" << be_idt_nl
      << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Boolean result = true;" << be_nl_2;
  }
}

int
be_visitor_union_cdr_op_cs::visit_union (be_union *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  be_visitor_context ctx (*this->ctx_);
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_localtypes);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      be_enum *e = dynamic_cast<be_enum*> (d);
      if (e != nullptr)
        {
          be_visitor_enum_cdr_op_cs visitor (&ctx);

          if (e->accept (&visitor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%N:%l) be_visitor_union_cdr_op_cs::visit_union"
                          " - codegen for enum failed\n"));
            }

          // Restore the union node in the enum visitor's context.
          ctx.node (this->ctx_->node ());
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs"
                         "::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_union)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  serialize_disc (os, node->udisc_type (), true /* out */);

  const bool boolDisc = node->udisc_type () == AST_Expression::EV_bool;
  if (!boolDisc)
    {
      *os << "switch (_tao_union._d ())" << be_nl
          << "{" << be_idt;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (!boolDisc && node->gen_empty_default_label ())
    {
      *os << be_nl << "default:" << be_idt_nl;
      *os << "break;"<< be_uidt;
    }

  if (!boolDisc)
    {
      *os << be_uidt_nl << "}" << be_nl_2;
    }

  *os << "return result;" << be_uidt_nl
      << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->name () << " &_tao_union)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl;

  be_type* disc_type =
    dynamic_cast<be_type*> (node->disc_type ());

  // Generate a temporary to store the discriminant
  *os << disc_type->full_name ()
      << " " << "_tao_discriminant;" << be_nl;

  serialize_disc (os, node->udisc_type (), false /* in */);

  if (boolDisc)
    {
      if (node->gen_empty_default_label ())
        {
          *os << "_tao_union._default ();" << be_nl
              << "_tao_union._d (_tao_discriminant);" << be_nl;
        }
    }
  else
    {
      *os << "switch (_tao_discriminant)" << be_nl
          << "{" << be_idt;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (!boolDisc && node->gen_empty_default_label ())
    {
      *os << be_nl;
      *os << "default:" << be_idt_nl;
      *os << "_tao_union._default ();" << be_nl;
      *os << "// For maximum interop compatibility, force the same value as transmitted" << be_nl;
      *os << "_tao_union._d (_tao_discriminant);" << be_nl;
      *os << "break;" << be_uidt;
    }

  if (!boolDisc)
    {
      *os << be_uidt_nl << "}" << be_nl_2;
    }

  *os << "return result;" << be_uidt_nl
      << "}" << be_nl;

  bool use_underscore = (this->ctx_->tdef () == nullptr);

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, use_underscore);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (true);
  return 0;
}
