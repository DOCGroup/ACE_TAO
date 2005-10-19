//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op.cpp
//
// = DESCRIPTION
//    Visitor generating code for the CDR operators for types defined in Module's
//    scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_module, 
           cdr_op, 
           "$Id: cdr_op.cpp,v 1.2 1999/05/28 02:16:43 coryan Exp ")

// ***************************************************************************
// Module visitor for generating CDR operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_module_cdr_op::be_visitor_module_cdr_op (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_cdr_op::~be_visitor_module_cdr_op (void)
{
}

int
be_visitor_module_cdr_op::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module_cdr_op::visit_module - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}
