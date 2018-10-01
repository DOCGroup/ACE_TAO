#include "ast_annotation.h"

AST_Decl::NodeType const AST_Annotation::NT = AST_Decl::NT_annotation;

AST_Annotation::AST_Annotation (UTL_ScopedName *name)
  : AST_Decl (NT, name), name_ (name->copy())
{
}

AST_Annotation::~AST_Annotation ()
{
  delete name_;
}

void AST_Annotation::dump (ACE_OSTREAM_TYPE &o)
{
  dump_i (o, "@");
  name_->dump(o);
  dump_i (o, "\n");
}

int AST_Annotation::ast_accept (ast_visitor *visitor)
{
  return 0;
}

void AST_Annotation::destroy ()
{
}
