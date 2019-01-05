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

  /// Narrowing
  ///{
  DEF_NARROW_FROM_DECL (AST_Annotation_Decl);
  DEF_NARROW_FROM_SCOPE (AST_Annotation_Decl);
  ///}

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Cleanup
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

  /**
   * Work around for AST node lookup, so that annotations do not conflict with
   * other IDL types and variables that have the same name. This is done by
   * prepending '@' to all annotation names.
   */
  ///{
  static void escape_name (Identifier *name);
  static void escape_name (UTL_ScopedName *name);
  ///}

  virtual bool annotatable () const;

  virtual AST_Annotation_Member *fe_add_annotation_member (
    AST_Annotation_Member *annotation_member);

  virtual AST_Constant *fe_add_constant (AST_Constant *t);

  virtual int ast_accept (ast_visitor *visitor);

  /**
   * Should AMI visit this node? (NO)
   */
  virtual bool ami_visit ();
};

#endif
