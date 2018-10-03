#include "ast_annotation.h"

AST_Decl::NodeType const AST_Annotation::NT = AST_Decl::NT_annotation;

AST_Annotation::AST_Annotation (UTL_ScopedName *name, AST_Annotation::Params *params)
  : AST_Decl (NT, name), name_ (name->copy ()), params_ (params)
{
}

AST_Annotation::~AST_Annotation ()
{
  delete name_;
}

void AST_Annotation::dump (ACE_OSTREAM_TYPE &o)
{
  dump_i (o, "@");
  name_->dump (o);
  if (params_)
    {
      dump_i (o, "(");
      size_t param_count = params_->size ();
      for (size_t i = 0; i < param_count; i++)
        {
          Param *p;
          params_->get (p, i);
          if (p->id)
            {
              p->id->dump (o);
              dump_i (o, " = ");
            }
          p->expr->dump (o);
          if (i != (param_count - 1))
            {
              dump_i (o, ", ");
            }
        }
      dump_i (o, ")");
    }
  dump_i (o, "\n");
}

int AST_Annotation::ast_accept (ast_visitor *visitor)
{
  return 0;
}

void AST_Annotation::destroy ()
{
}
