//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    discriminant_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for discriminant of the union.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           discriminant_ci, 
           "$Id$")

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
    this->ctx_->be_node_as_union ();
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_discriminant_ci::"
                         "visit_enum - "
                         "computing default value failed\n"),
                        -1);
    }

  if ((dv.computed_ != 0) && (bu->default_index () == -1))
    {
      // only if all cases are not covered AND there is no explicit
      // default, we get the _default () method
      os->indent ();
      *os << "// the implicit _default () method" << be_nl;
      *os << "ACE_INLINE void " << be_nl
          << bu->name () << "::_default ()" << be_nl
          << "{" << be_idt_nl
          << "this->disc_ = ";

      be_type* dt =
        be_type::narrow_from_decl (bu->disc_type ());

      if (dt == 0)
        {
          return -1;
        }

      // Find where was the enum defined, if it was defined in the globa
      // scope, then it is easy to generate the enum values....
      be_scope* scope =
        be_scope::narrow_from_scope (dt->defined_in ());

      if (scope == 0)
        {
          *os << node->value_to_name (dv.u.enum_val);
          return 0;
        }

      // The function value_to_name() takes care of adding
      // any necessary scoping to the output.
      *os << node->value_to_name (dv.u.enum_val);
      *os << ";" << be_uidt_nl << "}\n\n";
    }

  os->indent ();
  // the set method
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // the get method
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_union_discriminant_ci::visit_predefined_type (be_predefined_type
                                                         *node)
{
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

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
                         "(%N:%l) be_visitor_union_discriminant_ci::"
                         "visit_enum - "
                         "computing default value failed\n"),
                        -1);
    }

  if ((dv.computed_ != 0) && (bu->default_index () == -1))
    {
      // Only if all cases are not covered AND there is no explicit
      // default, we get the _default () method.
      os->indent ();

      *os << "// the implicit _default () method" << be_nl;
      *os << "ACE_INLINE void " << be_nl
          << bu->name () << "::_default ()" << be_nl
          << "{" << be_idt_nl
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
          os->print ("'\\%d'", dv.u.char_val);
          break;
        case AST_Expression::EV_wchar:
          os->print ("L'\\%d'", dv.u.wchar_val);
          break;
        case AST_Expression::EV_bool:
          *os << dv.u.bool_val;

          break;
        case AST_Expression::EV_longlong:
        case AST_Expression::EV_ulonglong:
          // Unimplemented.
        default:
          // Error caught earlier.
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ci::"
                             "visit_predefined_type - "
                             "bad or unimplemented discriminant type\n"),
                        -1);
        }

      *os << ";" << be_uidt_nl << "}\n\n";
    }

  os->indent ();

  // The set method.
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // The get method.
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";

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
                         "(%N:%l) be_visitor_union_discriminant_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"), 
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
