
//=============================================================================
/**
 *  @file    union_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Unions in the client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


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

  // Evaluate the member in time for the decision to generate
  // the recursive typecode include in the stub source file.
  ACE_Unbounded_Queue<AST_Type *> list;
  (void) node->in_recursion (list);

  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate _var and _out class typedefs.
  node->gen_common_varout (os);

  *os << be_nl_2
      << "class " << be_global->stub_export_macro () << " "
      << node->local_name () << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl

    // Generate default and copy constructors.
      << node->local_name () << " (void);" << be_nl
      << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl
    // Generate destructor.
      << "~" << node->local_name () << " (void);";

    // Generate assignment operator.
  *os << be_nl_2
      << node->local_name () << " &operator= (const "
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
                         ACE_TEXT ("be_visitor_union_ch::")
                         ACE_TEXT (" visit_union - ")
                         ACE_TEXT ("codegen for discriminant failed\n")),
                        -1);
    }

  node->gen_stub_decls (os);

  // Now generate the public defn for the union branch members. For this,
  // set our state to reflect what we are aiming to do.
  this->ctx_->state (TAO_CodeGen::TAO_UNION_PUBLIC_CH);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_ch::")
                         ACE_TEXT ("visit_union - ")
                         ACE_TEXT ("codegen for public ")
                         ACE_TEXT ("defn of union members\n")),
                        -1);
    }

  // Now check if we need to generate the _default () method.
  be_union::DefaultValue dv;

  if (node->default_value (dv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_ch::")
                         ACE_TEXT ("visit_union - ")
                         ACE_TEXT ("computing default ")
                         ACE_TEXT ("value failed\n")),
                        -1);
    }

  if ((dv.computed_ != 0) && (node->default_index () == -1))
    {
      *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      // Only if all cases are not covered AND there is no explicit
      // default, we get the _default () method.
      *os << be_nl_2
          << "void _default (void);";
    }

  *os << be_uidt_nl;

  // Now generate the private data members of the union.
  *os << "private:" << be_idt_nl;
  *os << bt->nested_type_name (node) << " disc_;" << be_nl_2;

  // The members are inside of a union.
  *os << "union" << be_nl;
  *os << "{" << be_idt;

  this->ctx_->state (TAO_CodeGen::TAO_UNION_PRIVATE_CH);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_union_ch::")
                         ACE_TEXT ("visit_union - ")
                         ACE_TEXT ("codegen for private")
                         ACE_TEXT (" members of union\n")),
                        -1);
    }

  *os << be_uidt_nl;
  *os << "} u_;";

  // The reset method (TAO extension).
  *os << be_nl_2
      << "/// TAO extension - frees any allocated storage." << be_nl;
  *os << "void _reset (void);";

  *os << be_uidt_nl << "};";

  if (be_global->tc_support ())
    {
      ctx = *this->ctx_;
      be_visitor_typecode_decl tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_union_ch::")
                             ACE_TEXT ("visit_union - ")
                             ACE_TEXT ("TypeCode declaration failed\n")),
                            -1);
        }
    }

  node->cli_hdr_gen (true);
  return 0;
}
