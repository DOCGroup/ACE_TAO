#include "ast_annotation_appl.h"

AST_Decl::NodeType const AST_Annotation_Appl::NT = AST_Decl::NT_annotation_appl;

AST_Annotation_Appl::AST_Annotation_Appl (
  UTL_ScopedName *name, AST_Annotation_Appl::Params *params)
  : AST_Decl (NT, name),
    AST_Type (NT, name),
    AST_ConcreteType (NT, name),
    UTL_Scope (NT),
    AST_Structure (name, false, false),
    AST_Annotation_Decl (name),
    original_name_ (name->get_string_copy ()),
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
      Params::ITERATOR iter (*params_);
      Param **i;
      while (!iter.done ())
        {
          iter.next (i);
          if ((*i)->id)
            {
              (*i)->id->dump (o);
              dump_i (o, " = ");
            }
          (*i)->expr->dump (o);
          iter.advance ();
          if (!iter.done ())
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

IMPL_NARROW_FROM_DECL (AST_Annotation_Appl)
IMPL_NARROW_FROM_SCOPE (AST_Annotation_Appl)
