#ifndef BE_ANNOTATION_DECL_HEADER
#define BE_ANNOTATION_DECL_HEADER

class be_visitor;

#include "ast_annotation_decl.h"

class be_annotation_decl : public virtual AST_Annotation_Decl
{
public:
  be_annotation_decl (UTL_ScopedName *name);

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_FROM_DECL (be_annotation_decl);
  DEF_NARROW_FROM_SCOPE (be_annotation_decl);
};

#endif
