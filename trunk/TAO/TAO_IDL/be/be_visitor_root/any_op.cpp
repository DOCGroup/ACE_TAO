//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Any operators for types defined in Root's
//    scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           any_op, 
           "$Id$")

// ***************************************************************************
// Root visitor for generating Any operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_root_any_op::be_visitor_root_any_op (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_any_op::~be_visitor_root_any_op (void)
{
}

int
be_visitor_root_any_op::visit_root (be_root *node)
{
  if (be_global->gen_anyop_files ())
    { 
      // Switch streams, ctx will be reassigned when this
      // pass is done.
      switch (this->ctx_->state ())
        {
          case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
            this->ctx_->stream (tao_cg->anyop_header ());
            break;         
          case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
            this->ctx_->stream (tao_cg->anyop_source ());
            break;         
          default:
            break;
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::visit_root - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}
