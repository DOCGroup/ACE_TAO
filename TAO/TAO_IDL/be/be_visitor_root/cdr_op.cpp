
//=============================================================================
/**
 *  @file    cdr_op.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the CDR operators for types defined
 *  in Root's scope.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "root.h"

// ***************************************************************************
// Root visitor for generating CDR operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_root_cdr_op::be_visitor_root_cdr_op (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cdr_op::~be_visitor_root_cdr_op (void)
{
}

int
be_visitor_root_cdr_op::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cdr_op::")
                         ACE_TEXT ("visit_root - codegen ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  return 0;
}
