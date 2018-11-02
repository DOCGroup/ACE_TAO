/**
 * Abstract Syntax Tree Node for a declaration of an annotation.
 */

#ifndef AST_ANNOTATION_DECL_HEADER
#define AST_ANNOTATION_DECL_HEADER

#include "ast_structure.h"

/**
 * Abstract Syntax Tree Node for a declaration of an annotation.
 */
class TAO_IDL_FE_Export AST_Annotation_Decl : public virtual AST_Structure
{
public:

  AST_Annotation_Decl (UTL_ScopedName *name);

  virtual ~AST_Annotation_Decl ();

  // Narrowing
  DEF_NARROW_FROM_DECL (AST_Annotation_Decl);
  DEF_NARROW_FROM_SCOPE (AST_Annotation_Decl);

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Cleanup
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

};

#endif
