//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for Union.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union,
           any_op_ch,
           "$Id$")

// ***************************************************************************
// Union visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_union_any_op_ch::be_visitor_union_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_ch::~be_visitor_union_any_op_ch (void)
{
}

int
be_visitor_union_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << be_global->core_versioning_begin () << be_nl;
  
  *os << macro << " void operator<<= (::CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&); // deprecated\n";
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const "
      << node->name () << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_union_any_op_ch::visit_union_branch (be_union_branch *node)
{
  // First generate the type information.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_union_branch - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_ch::visit_enum (be_enum *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_enum_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_structure_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_structure - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

