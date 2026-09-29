
//=============================================================================
/**
 *  @file    discriminant_ci.cpp
 *
 *  Visitor generating code for discriminant of the union.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

namespace
{
  int
  generate_member_selection (TAO_OutStream *os,
                             be_union *node)
  {
    unsigned long default_member = node->nfields ();

    *os << "{" << be_idt_nl;

    if (node->udisc_type () == AST_Expression::EV_bool)
      {
        unsigned long false_member = node->nfields ();
        unsigned long true_member = node->nfields ();

        for (unsigned long i = 0; i < node->nfields (); ++i)
          {
            AST_Field **field = nullptr;

            if (node->field (field, i) != 0 || field == nullptr)
              {
                return -1;
              }

            be_union_branch *branch = dynamic_cast<be_union_branch*> (*field);

            if (branch == nullptr)
              {
                continue;
              }

            for (unsigned long j = 0; j < branch->label_list_length (); ++j)
              {
                AST_UnionLabel *label = branch->label (j);

                if (label->label_kind () == AST_UnionLabel::UL_default)
                  {
                    default_member = i;
                  }
                else if (label->label_val ()->ev ()->u.bval)
                  {
                    true_member = i;
                  }
                else
                  {
                    false_member = i;
                  }
              }
          }

        if (false_member == node->nfields ())
          {
            false_member = default_member;
          }

        if (true_member == node->nfields ())
          {
            true_member = default_member;
          }

        *os << "return value ? " << true_member << "UL : "
            << false_member << "UL;" << be_uidt_nl
            << "}";
        return 0;
      }

    *os << "switch (value)" << be_idt_nl
        << "{" << be_idt_nl;

    for (unsigned long i = 0; i < node->nfields (); ++i)
      {
        AST_Field **field = nullptr;

        if (node->field (field, i) != 0 || field == nullptr)
          {
            return -1;
          }

        be_union_branch *branch = dynamic_cast<be_union_branch*> (*field);

        if (branch == nullptr)
          {
            continue;
          }

        bool generated_label = false;
        for (unsigned long j = 0; j < branch->label_list_length (); ++j)
          {
            if (branch->label (j)->label_kind () == AST_UnionLabel::UL_default)
              {
                default_member = i;
              }
            else
              {
                *os << "case ";
                branch->gen_label_value (os, j);
                *os << ":" << be_nl;
                generated_label = true;
              }
          }

        if (generated_label)
          {
            *os << be_idt << "return " << i << "UL;" << be_uidt_nl;
          }
      }

    *os << "default:" << be_idt_nl
        << "return " << default_member << "UL;" << be_uidt_nl
        << be_uidt << "}" << be_uidt_nl
        << "}";

    return 0;
  }

  int
  generate_discriminator_validation (TAO_OutStream *os, be_union *node)
  {
    if (node->nfields () == 1
        && (node->default_index () != -1 || !node->gen_empty_default_label ()))
      {
        return 0;
      }

    *os << "if (this->disc_ != discval)" << be_idt_nl
        << "{" << be_idt_nl;

    *os << "if (_tao_member_index (this->disc_) != "
        << "_tao_member_index (discval))" << be_idt_nl
        << "{" << be_idt_nl
        << "throw ::CORBA::BAD_PARAM ();" << be_uidt_nl
        << "}" << be_uidt_nl
        << be_uidt << "}" << be_uidt_nl;

    return 0;
  }
}

be_visitor_union_discriminant_ci::be_visitor_union_discriminant_ci (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ci::~be_visitor_union_discriminant_ci ()
{
}

int
be_visitor_union_discriminant_ci::visit_enum (be_enum *node)
{
  be_union *bu =
    dynamic_cast<be_union*> (this->ctx_->node ());
  be_type *bt = nullptr;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // now check if we need to generate the _default () method
  be_union::DefaultValue dv;

  if (bu->default_value (dv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_discriminant_ci::")
                         ACE_TEXT ("visit_enum - ")
                         ACE_TEXT ("computing default value failed\n")),
                        -1);
    }

  TAO_INSERT_COMMENT (os);

  if ((dv.computed_ != 0) && (bu->default_index () == -1))
    {
      // Only if all cases are not covered AND there is no explicit
      // default, we get the _default () method.
      *os << "ACE_INLINE" << be_nl
          << "void" << be_nl
          << bu->name () << "::_default ()" << be_nl
          << "{" << be_idt_nl
          << "this->_reset ();" << be_nl
          << "this->disc_ = ";

      // We use one of the enum values that isn't used in this
      // union if one is available.
      UTL_ScopedName *sn = node->value_to_name (dv.u.enum_val);

      if (sn)
        {
          // The function value_to_name() takes care of adding
          // any necessary scoping to the output.
          *os << sn;
        }
      else
        {
          // Since CORBA defines enums to be 32bits, use -1 as the
          // out-of-bounds value for the _default() function.
          *os << "static_cast <" << bt->name () << "> (-1)";
        }

      *os << ";" << be_uidt_nl
          << "}" << be_nl_2;
    }

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << "unsigned long" << be_nl
      << bu->name () << "::_tao_member_index (" << bt->name ()
      << " value)" << be_nl;

  if (generate_member_selection (os, bu) == -1)
    {
      return -1;
    }

  // the set method
  *os << be_nl_2
      << "// Accessor to set the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl;

  if (generate_discriminator_validation (os, bu) == -1)
    {
      return -1;
    }

  *os << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl_2;

  // the get method
  *os << "// Accessor to get the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << be_nl
      << bu->name () << "::_d () const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_discriminant_ci::visit_predefined_type (
    be_predefined_type *node
  )
{
  be_union *bu =
    dynamic_cast<be_union*> (this->ctx_->node ());

  be_type *bt = nullptr;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Now check if we need to generate the _default () method.
  be_union::DefaultValue dv;

  if (bu->default_value (dv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_discriminant_ci::")
                         ACE_TEXT ("visit_enum - ")
                         ACE_TEXT ("computing default value failed\n")),
                        -1);
    }

  TAO_INSERT_COMMENT (os);

  if ((dv.computed_ != 0) && (bu->default_index () == -1))
    {
      // Only if all cases are not covered AND there is no explicit
      // default, we get the _default () method.

      *os << "ACE_INLINE" << be_nl
          << "void" << be_nl
          << bu->name () << "::_default ()" << be_nl
          << "{" << be_idt_nl
          << "this->_reset ();" << be_nl
          << "this->disc_ = ";

      switch (bu->udisc_type ())
        {
        case AST_Expression::EV_short:
          *os << dv.u.short_val;
          break;
        case AST_Expression::EV_ushort:
        case AST_Expression::EV_wchar:
          *os << dv.u.ushort_val;
          break;
        case AST_Expression::EV_long:
          *os << dv.u.long_val;
          break;
        case AST_Expression::EV_ulong:
          *os << dv.u.ulong_val;
          break;
        case AST_Expression::EV_char:
        case AST_Expression::EV_octet:
        case AST_Expression::EV_int8:
        case AST_Expression::EV_uint8:
          os->print ("'\\%o'", dv.u.char_val);
          break;
        case AST_Expression::EV_bool:
          *os << (dv.u.bool_val == 0 ? "false" : "true");
          break;
        case AST_Expression::EV_longlong:
          *os << dv.u.longlong_val;
          break;
        case AST_Expression::EV_ulonglong:
          *os << dv.u.ulonglong_val;
          break;
        default:
          // Error caught earlier.
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_union_discriminant_ci::")
                             ACE_TEXT ("visit_predefined_type - ")
                             ACE_TEXT ("bad or unimplemented ")
                             ACE_TEXT ("discriminant type\n")),
                            -1);
        }

      *os << ";" << be_uidt_nl << "}";
    }

  *os << be_nl_2
      << "ACE_INLINE" << be_nl
      << "unsigned long" << be_nl
      << bu->name () << "::_tao_member_index (::" << bt->name ()
      << " value)" << be_nl;

  if (generate_member_selection (os, bu) == -1)
    {
      return -1;
    }

  // The set method.
  *os << be_nl_2
      << "// Accessor to set the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::_d (::" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl;

  if (generate_discriminator_validation (os, bu) == -1)
    {
      return -1;
    }

  *os << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl_2;

  // The get method.
  *os << "// Accessor to get the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "::" << bt->name () << be_nl
      << bu->name () << "::_d () const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_union_discriminant_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_discriminant_ci::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("Bad primitive type\n")),
                        -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}
