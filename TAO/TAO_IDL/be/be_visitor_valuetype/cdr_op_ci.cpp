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
  os->indent (); //start with whatever indentation level we are at now

  // This is just declaration so no ACE_INLINE
  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);" << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_inline_cdr_op_gen (I_TRUE);
  return 0;
}
