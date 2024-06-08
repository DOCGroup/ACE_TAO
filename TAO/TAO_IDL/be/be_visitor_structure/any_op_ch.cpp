
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for structures
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "structure.h"

// ***************************************************************************
// Structure visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_structure_any_op_ch::be_visitor_structure_any_op_ch (
                                                                be_visitor_context *ctx
                                                                )
  : be_visitor_structure (ctx)
{
}

int
be_visitor_structure_any_op_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, const ::" << node->name ()
      << " &); // copying version" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, ::" << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const ::"
      << node->name () << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_any_op_gen (true);
  return 0;
}

int
be_visitor_structure_any_op_ch::visit_field (be_field *node)
{
  be_type *bt = nullptr;

  // First generate the type information.
  bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_structure_any_op_ch::visit_union (be_union *node)
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
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_union - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_structure_any_op_ch::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_structure_any_op_ch::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}
