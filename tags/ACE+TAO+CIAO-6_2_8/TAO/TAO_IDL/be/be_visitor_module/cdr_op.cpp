
//=============================================================================
/**
 *  @file    cdr_op.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the CDR operators for types defined in Module's
 *  scope.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "module.h"

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
