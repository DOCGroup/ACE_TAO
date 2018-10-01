/**
 * Abstract Syntax Tree Node for an application of an annotation.
 */

#ifndef AST_ANNOTATION_HEADER
#define AST_ANNOTATION_HEADER

#include "ast_decl.h"

#include "ace/Containers.h"

/**
 * Abstract Syntax Tree Node for an application of an annotation.
 */
class TAO_IDL_FE_Export AST_Annotation : public virtual AST_Decl
{
public:

  /**
   * Create an Annotation using it's name.
   */
  AST_Annotation (UTL_ScopedName *name);

  virtual ~AST_Annotation ();

  // Narrowing
  DEF_NARROW_FROM_DECL (AST_Annotation);

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Visiting
  virtual int ast_accept (ast_visitor *visitor);

  /// Cleanup
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

private:

  /// Name of the Annotation
  UTL_ScopedName *name_;
};

typedef ACE_DLList<AST_Annotation> Annotations;
typedef ACE_DLList_Iterator<AST_Annotation> AnnotationsIter;

#endif
