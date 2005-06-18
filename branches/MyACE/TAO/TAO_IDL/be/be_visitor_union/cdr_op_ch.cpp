//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for unions. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           cdr_op_ch, 
           "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_union_cdr_op_ch::be_visitor_union_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_ch::~be_visitor_union_cdr_op_ch (void)
{
}

int
be_visitor_union_cdr_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_cdr_op_gen () 
      || node->imported () 
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator<< (TAO_OutputCDR &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>> (TAO_InputCDR &, "
      << node->name () << " &);";


  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ch::"
                         "visit_union - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
