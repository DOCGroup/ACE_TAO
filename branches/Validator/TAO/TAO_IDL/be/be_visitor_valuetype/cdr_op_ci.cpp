//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           cdr_op_ci, 
           "$Id$")

be_visitor_valuetype_cdr_op_ci::be_visitor_valuetype_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_ci::~be_visitor_valuetype_cdr_op_ci (void)
{
}

int
be_visitor_valuetype_cdr_op_ci::visit_valuetype (be_valuetype *node)
{
  // Already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  // Generate helper functions implementation.
  if (node->gen_helper_inline () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"), 
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // This is just declaration so no ACE_INLINE
  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);";

  node->cli_inline_cdr_op_gen (I_TRUE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}

// @@@ (JP) The following three methods are a hack to get CDR
// operators generated for anonymous array and sequence 
// valuetype members. This should be done like it is in structs,
// but part of that mechanism is used by valuetypes for 
// generating code to marshal the state. Someday this should
// be untangled and made consistent.

int
be_visitor_valuetype_cdr_op_ci::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_ci::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // Save the node.
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_cdr_op_ci::visit_array (be_array *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_array_cdr_op_ci visitor (&ctx);
  return node->accept (&visitor);
}

int
be_visitor_valuetype_cdr_op_ci::visit_sequence (be_sequence *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_cdr_op_ci visitor (&ctx);
  return node->accept (&visitor);
}

