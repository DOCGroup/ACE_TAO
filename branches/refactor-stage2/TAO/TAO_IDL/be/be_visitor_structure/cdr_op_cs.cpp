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
//    Visitor generating code for CDR operators for structures
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_structure, 
           cdr_op_cs, 
           "$Id$")

be_visitor_structure_cdr_op_cs::be_visitor_structure_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_cdr_op_cs::~be_visitor_structure_cdr_op_cs (void)
{
}

int
be_visitor_structure_cdr_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_cdr_op_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
