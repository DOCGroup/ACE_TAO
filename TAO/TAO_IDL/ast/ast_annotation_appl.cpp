#include "ast_annotation_appl.h"

AST_Decl::NodeType const AST_Annotation_Appl::NT = AST_Decl::NT_annotation_appl;

AST_Annotation_Appl::AST_Annotation_Appl (
  UTL_ScopedName *name, AST_Annotation_Appl::Params *params)
  : AST_Decl (NT, name), original_name_ (name->get_string_copy ()),
    params_ (params)
{
}

AST_Annotation_Appl::~AST_Annotation_Appl ()
{
  delete [] original_name_;
}

void AST_Annotation_Appl::dump (ACE_OSTREAM_TYPE &o)
{
  dump_i (o, "@");
  dump_i (o, original_name_);
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
}

int AST_Annotation_Appl::ast_accept (ast_visitor *visitor)
{
  ACE_UNUSED_ARG (visitor);
  return 0;
}

void AST_Annotation_Appl::destroy ()
{
}

const char *AST_Annotation_Appl::original_name () const
{
  return original_name_;
}
