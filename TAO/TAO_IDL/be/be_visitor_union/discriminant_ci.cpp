
//=============================================================================
/**
 *  @file    discriminant_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for discriminant of the union.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// *************************************************************************
// be_visitor_discriminant_ci - visitor for discriminant in client inline file
// *************************************************************************

be_visitor_union_discriminant_ci::be_visitor_union_discriminant_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ci::~be_visitor_union_discriminant_ci (void)
{
}

int
be_visitor_union_discriminant_ci::visit_enum (be_enum *node)
{
  be_union *bu =
    be_union::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

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

  // the set method
  *os << "// Accessor to set the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl_2;

  // the get method
  *os << "// Accessor to get the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
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
    be_union::narrow_from_decl (this->ctx_->node ());

  be_type *bt = 0;

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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

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
          *os << dv.u.ushort_val;
          break;
        case AST_Expression::EV_long:
          *os << dv.u.long_val;
          break;
        case AST_Expression::EV_ulong:
          *os << dv.u.ulong_val;
          break;
        case AST_Expression::EV_char:
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

  // The set method.
  *os << be_nl_2
      << "// Accessor to set the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "void" << be_nl
      << bu->name () << "::_d ( ::" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl_2;

  // The get method.
  *os << "// Accessor to get the discriminant." << be_nl
      << "ACE_INLINE" << be_nl
      << "::" << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
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

  this->ctx_->alias (0);
  return 0;
}
