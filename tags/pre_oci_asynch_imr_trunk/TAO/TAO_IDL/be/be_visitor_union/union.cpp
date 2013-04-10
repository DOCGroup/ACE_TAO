
//=============================================================================
/**
 *  @file    union.cpp
 *
 *  $Id$
 *
 *  Generic visitor generating code for Unions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union::be_visitor_union (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_union::~be_visitor_union (void)
{
}

int
be_visitor_union::visit_union (be_union *)
{
  return -1; // Must be overriden.
}

int
be_visitor_union::visit_union_branch (be_union_branch *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
      {
        be_visitor_union_branch_public_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
      {
        be_visitor_union_branch_private_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_union_branch_public_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      {
        be_visitor_union_branch_public_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      {
        be_visitor_union_branch_public_assign_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS:
      {
        be_visitor_union_branch_public_reset_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_union_branch_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_union_branch_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_union::"
                           "visit_union_branch - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::"
                         "visit_union_branch - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_cdr_op_cs::pre_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    {
      return 0;
    }

  // Enum val nodes are added just to help check reference
  // clashes, since an enum declared in our scope is not itself
  // a scope.
  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_union_branch* b =
    be_union_branch::narrow_from_decl (bd);

  // Could be a type decl.
  if (b == 0)
    {
      return 0;
    }

  *os << be_nl;

  for (unsigned long i = 0; i < b->label_list_length (); ++i)
    {
      // check if we are printing the default case
      if (b->label (i)->label_kind () == AST_UnionLabel::UL_default)
        {
          *os << "default:";
        }
      else
        {
          *os << "case ";

          b->gen_label_value (os, i);

          *os << ":";
        }

      if (i == (b->label_list_length () - 1))
        {
          *os << be_idt_nl;
        }
      else
        {
          *os << be_nl;
        }
    }

  *os << "{" << be_idt_nl;

  return 0;
}

int
be_visitor_union_cdr_op_cs::post_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    {
      return 0;
    }

  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << "}" << be_nl
      << "break;" << be_uidt;

  return 0;
}
