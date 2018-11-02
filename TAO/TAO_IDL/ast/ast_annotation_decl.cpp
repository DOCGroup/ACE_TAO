#include "ast_annotation_decl.h"
#include "utl_indenter.h"

AST_Decl::NodeType const AST_Annotation_Decl::NT =
  AST_Decl::NT_annotation_decl;

AST_Annotation_Decl::AST_Annotation_Decl (UTL_ScopedName *name)
  : AST_Decl (NT, name),
    AST_Type (NT, name),
    AST_ConcreteType (NT, name),
    UTL_Scope (NT),
    AST_Structure (name, false, false)
{
}

AST_Annotation_Decl::~AST_Annotation_Decl ()
{
}

void AST_Annotation_Decl::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "@annotation ");
  AST_Decl::dump (o);
  this->dump_i (o, " {\n");
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  this->dump_i (o, "}");
}

void AST_Annotation_Decl::destroy ()
{
  AST_Decl::destroy ();
}

IMPL_NARROW_FROM_DECL (AST_Annotation_Decl)
IMPL_NARROW_FROM_SCOPE (AST_Annotation_Decl)
