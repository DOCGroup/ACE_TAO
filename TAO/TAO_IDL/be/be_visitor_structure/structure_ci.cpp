
//=============================================================================
/**
 *  @file    structure_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Structure in the inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "structure.h"

// ******************************************************
// for client inline
// ******************************************************

be_visitor_structure_ci::be_visitor_structure_ci (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_ci::~be_visitor_structure_ci (void)
{
}

// visit the Structure node and its scope
int be_visitor_structure_ci::visit_structure (be_structure *node)
{
  if (node->cli_inline_gen () || node->imported ())
    {
      return 0;
    }

  // All we have to do is to visit the scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_ci::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_inline_gen (true);
  return 0;
}
