//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Unions in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           union_ch, 
           "$Id$")

// ******************************************************
// For client header.
// ******************************************************

be_visitor_union_ch::be_visitor_union_ch (be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_ch::~be_visitor_union_ch (void)
{
}

int be_visitor_union_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate _var and _out class typedefs.
  node->gen_common_varout (os);

  // Generate the ifdefined macro for the union type.
  os->gen_ifdef_macro (node->flat_name ());

  *os << be_nl << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << node->local_name () << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl

    // Generate default and copy constructors.
      << node->local_name () << " (void);" << be_nl
      << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl
    // Generate destructor.
      << "~" << node->local_name () << " (void);" << be_nl;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void*);"
          << be_nl << be_nl;
    }

    // Generate assignment operator.
  *os << node->local_name () << " &operator= (const "
      << node->local_name () << " &);";

  // Retrieve the disriminant type.
  be_type *bt = be_type::narrow_from_decl (node->disc_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         "visit_union - "
                         "bad disciminant type\n"), 
                        -1);
    }

  // The discriminant type may have to be defined here if it was an enum
  // declaration inside of the union statement.

  be_visitor_union_discriminant_ch ud_visitor (&ctx);

  if (bt->accept (&ud_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         " visit_union - "
                         "codegen for discriminant failed\n"), 
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the _var_type typedef.
  *os << be_nl << be_nl 
      << "typedef " << node->local_name () << "_var _var_type;";

  // Now generate the public defn for the union branch members. For this,
  // set our state to reflect what we are aiming to do.
  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         "visit_union - "
                         "codegen for public defn of union members\n"),
                        -1);
    }

  // Now check if we need to generate the _default () method.
  be_union::DefaultValue dv;

  if (node->default_value (dv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         "visit_union - "
                         "computing default value failed\n"),
                        -1);
    }

  if ((dv.computed_ != 0) && (node->default_index () == -1))
    {
      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      // Only if all cases are not covered AND there is no explicit
      // default, we get the _default () method.
      *os << be_nl << be_nl 
          << "void _default (void);";
    }

  *os << be_uidt_nl;

  // Now generate the private data members of the union.
  *os << "private:" << be_idt_nl;
  *os << bt->nested_type_name (node) << " disc_;" << be_nl;
  *os << bt->nested_type_name (node) << " holder_;" << be_nl << be_nl;
  // Emit the ACE_NESTED_CLASS macro.

  // The members are inside of a union.
  *os << "union" << be_nl;
  *os << "{" << be_idt;

  this->ctx_->state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         "visit_union - "
                         "codegen for private members of union\n"), 
                        -1);
    }

  *os << be_uidt_nl;
  *os << "} u_;";

  // The reset method (TAO extension).
  *os << be_nl << be_nl 
      << "// TAO extension - frees any allocated storage." << be_nl;
  *os << "void _reset (" << bt->nested_type_name (node)
      << ", CORBA::Boolean /* finalize */);";

  *os << be_uidt_nl << "};";

  if (be_global->tc_support ())
    {
      ctx = *this->ctx_;
      be_visitor_typecode_decl tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_ch::"
                             "visit_union - "
                             "TypeCode declaration failed\n"), 
                            -1);
        }
    }

  os->gen_endif ();

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
