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
//    Visitor generating code for Any operators for elements in the scope of
//    the Module.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_module, 
           any_op, 
           "$Id$")

// ***************************************************************************
// Module visitor for generating Any operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_module_any_op::be_visitor_module_any_op (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_any_op::~be_visitor_module_any_op (void)
{
}

int
be_visitor_module_any_op::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::visit_module - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}
