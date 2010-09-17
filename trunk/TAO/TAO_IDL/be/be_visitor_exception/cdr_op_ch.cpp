
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for exceptions. This uses
 *  compiled marshaling.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Exception visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_exception_cdr_op_ch::be_visitor_exception_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cdr_op_ch::~be_visitor_exception_cdr_op_ch (void)
{
}

int
be_visitor_exception_cdr_op_ch::visit_exception (be_exception *node)
{
  if (node->cli_hdr_cdr_op_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_nl << be_nl
      << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO_OutputCDR &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO_InputCDR &, "
      << node->name () << " &);" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_global->stub_export_macro () << " std::ostream&"
          << " operator<< (std::ostream &strm, const "
          << node->name () << " &);" << be_nl;
    }

  *os << be_global->core_versioning_end () << be_nl;

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_cdr_op_ch::"
                         "visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
