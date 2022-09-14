#include "ast_visitor.h"
#include "ace/config-all.h"

ast_visitor::ast_visitor ()
{
}

ast_visitor::~ast_visitor ()
{
}

int ast_visitor::visit_fixed (AST_Fixed *)
{
  return 0;
}

int ast_visitor::visit_annotation_decl (AST_Annotation_Decl *)
{
  return 0;
}
