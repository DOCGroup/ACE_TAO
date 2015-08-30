
//=============================================================================
/**
 *  @file    union.cpp
 *
 *  Generic visitor generating code for Unions
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

be_visitor_union::BoolUnionBranch
be_visitor_union::boolean_branch (be_union_branch *b)
{
  be_union *u = be_union::narrow_from_scope (b->defined_in ());
  if (!u || u->udisc_type () != AST_Expression::EV_bool)
    return BUB_NONE;

  bool br_d = false, br_t = false, br_f = false;
  for (unsigned long i = 0; i < b->label_list_length (); ++i)
    if (b->label (i)->label_kind () == AST_UnionLabel::UL_default)
      br_d = true;
    else
      (b->label (i)->label_val ()->ev ()->u.bval ? br_t : br_f) = true;

  if ((br_t && br_f) || (u->nfields () == 1 && br_d))
    return BUB_UNCONDITIONAL;

  bool has_other = false, other_val = false;
  for (unsigned int i = 0; br_d && i < u->nfields (); ++i)
    {
      AST_Field **f;
      u->field (f, i);
      if (*f != b)
        {
          AST_UnionBranch *other = AST_UnionBranch::narrow_from_decl (*f);
          for (unsigned long j = 0; j < other->label_list_length (); ++j)
            {
              has_other = true;
              other_val = other->label (j)->label_val ()->ev ()->u.bval;
            }
        }
    }

  return (br_t || (has_other && !other_val)) ? BUB_TRUE : BUB_FALSE;
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

  this->latest_branch_ = boolean_branch (b);
  switch (this->latest_branch_)
    {
    case BUB_TRUE:
    case BUB_FALSE:
      *os << "if (" << (this->latest_branch_ == BUB_TRUE ? "" : "!")
          << (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT ?
              "_tao_union._d ()" : "_tao_discriminant") << ")" << be_idt_nl
          << "{" << be_idt_nl;
    case BUB_UNCONDITIONAL:
      return 0;
    default:
      break;
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

  switch (this->latest_branch_)
    {
    case BUB_NONE:
      *os << be_uidt_nl << "}" << be_nl << "break;" << be_uidt;
      break;
    case BUB_TRUE:
    case BUB_FALSE:
      *os << be_uidt_nl << "}" << be_uidt_nl << be_nl;
      break;
    case BUB_UNCONDITIONAL:
      *os << be_nl;
    }

  return 0;
}
