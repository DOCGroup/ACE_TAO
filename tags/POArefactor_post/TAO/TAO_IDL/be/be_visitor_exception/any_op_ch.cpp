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
//    Visitor generating code for the Any operators for Exceptions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_exception, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Exception visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_exception_any_op_ch::be_visitor_exception_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_any_op_ch::~be_visitor_exception_any_op_ch (void)
{
}

int
be_visitor_exception_any_op_ch::visit_exception (be_exception *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&); // deprecated\n";
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, const "
      << node->name () << " *&);";


  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::visit_exception - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_exception_any_op_ch::visit_field (be_field *node)
{
  // First generate the type information.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_enum - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_structure - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_union_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_union - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

