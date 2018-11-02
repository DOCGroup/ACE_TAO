#include "be_annotation_decl.h"

be_annotation_decl::be_annotation_decl (UTL_ScopedName *name)
  : AST_Decl (AST_Decl::NT_annotation_decl, name),
    AST_Type (AST_Decl::NT_annotation_decl, name),
    AST_ConcreteType (AST_Decl::NT_annotation_decl, name),
    AST_Structure (name, false, false),
    AST_Annotation_Decl (name)
{
}

int
be_annotation_decl::accept (be_visitor *visitor)
{
  return 0;
}

IMPL_NARROW_FROM_DECL (be_annotation_decl)
IMPL_NARROW_FROM_SCOPE (be_annotation_decl)
