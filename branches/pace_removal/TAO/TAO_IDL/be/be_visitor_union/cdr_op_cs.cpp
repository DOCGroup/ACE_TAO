//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for unions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union, 
           cdr_op_cs, 
           "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_cdr_op_cs::be_visitor_union_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_cs::~be_visitor_union_cdr_op_cs (void)
{
}

int
be_visitor_union_cdr_op_cs::visit_union (be_union *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen () 
      || node->imported () 
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs"
                         "::visit_union - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
