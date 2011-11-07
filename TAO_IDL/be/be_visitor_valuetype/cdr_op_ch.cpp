
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================


be_visitor_valuetype_cdr_op_ch::be_visitor_valuetype_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_ch::~be_visitor_valuetype_cdr_op_ch (void)
{
}

int
be_visitor_valuetype_cdr_op_ch::visit_valuetype (be_valuetype *node)
{
  if (node->imported ()
      || node->cli_hdr_cdr_op_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate helper functions declaration.
  if (node->gen_helper_header () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"),
                        -1);
    }

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "::CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "::CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_global->stub_export_macro () << " std::ostream&"
          << " operator<< (std::ostream &strm, const "
          << node->full_name () << " *);" << be_nl;
    }

  *os << be_global->core_versioning_end () << be_nl;

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"),
                        -1);
    }

  if (!node->is_abstract ())
    {
      // Functions that marshal state.
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_valuetype_marshal_ch visitor (&new_ctx);
      visitor.visit_valuetype (node);
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}

int
be_visitor_valuetype_cdr_op_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}
