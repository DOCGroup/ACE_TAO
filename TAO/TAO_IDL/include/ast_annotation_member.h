/**
 * Abstract Syntax Tree Node for a Member Value of an Annotation
 */

#ifndef AST_ANNOTATION_MEMBER_HEADER
#define AST_ANNOTATION_MEMBER_HEADER

#include "ast_field.h"
#include "ast_expression.h"

class AST_Expression;

/**
 * Abstract Syntax Tree Node for a Member Value of an Annotation
 */
class TAO_IDL_FE_Export AST_Annotation_Member : public virtual AST_Field
{
public:
  AST_Annotation_Member (
    AST_Expression::ExprType expr_type,
    AST_Type *type, UTL_ScopedName *name);

  AST_Annotation_Member (
    AST_Decl::NodeType nt,
    AST_Expression::ExprType expr_type,
    AST_Type *type, UTL_ScopedName *name);

  /// Pseudo-Copy Constructor for Creating Annotation Applications
  AST_Annotation_Member (
    UTL_ScopedName *name,
    AST_Annotation_Member *other);

  virtual ~AST_Annotation_Member ();

  static AST_Decl::NodeType const NT;

  /// Cleanup
  virtual void destroy ();

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Get and Set the Value
  ///{
  AST_Expression *value ();
  void value (AST_Expression *value);
  ///}

  /// Get the ExprType
  AST_Expression::ExprType expr_type ();

  /// Returns true if the expression value is null or invalid
  bool invalid_value ();

private:
  /// Keep ExprType at hand for convience
  AST_Expression::ExprType expr_type_;

  /// Value of the Member, can be null
  AST_Expression *value_;
};

#endif
