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
//    Visitor generating code for CDR operators for exceptions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_exception, 
           cdr_op_cs, 
           "$Id$")

be_visitor_exception_cdr_op_cs::be_visitor_exception_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cdr_op_cs::~be_visitor_exception_cdr_op_cs (void)
{
}

int
be_visitor_exception_cdr_op_cs::visit_exception (be_exception *node)
{
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_cdr_op_cs::"
                         "visit_exception - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
