//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    structure_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Structure in the inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_structure, 
           structure_ci, 
           "$Id$")

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
  if (!node->cli_inline_gen () && !node->imported ())
    {
      if (node->gen_var_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for _var failed\n"), -1);
        }
      if (node->size_type () == be_decl::VARIABLE &&
          node->gen_out_impl () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for _out failed\n"), -1);
        }
      // all we have to do is to visit the scope
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_ci::"
                             "visit_structure - "
                             "codegen for scope failed\n"), -1);
        }
      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}
