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

  ~AST_Annotation_Decl () override;

  /// AST Dumping
  void dump (ACE_OSTREAM_TYPE &o) override;

  /// Cleanup
  void destroy () override;

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

  bool annotatable () const override;

  AST_Annotation_Member *fe_add_annotation_member (
    AST_Annotation_Member *annotation_member) override;

  AST_Constant *fe_add_constant (AST_Constant *t) override;

  int ast_accept (ast_visitor *visitor) override;

  /**
   * Should AMI visit this node? (NO)
   */
  bool ami_visit () override;
};

#endif
