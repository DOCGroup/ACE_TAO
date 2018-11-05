/**
 * Abstract Syntax Tree Node for an application of an annotation.
 */

#ifndef AST_ANNOTATION_APPL_HEADER
#define AST_ANNOTATION_APPL_HEADER

#include "ast_annotation_decl.h"
#include "utl_identifier.h"
#include "ast_expression.h"

#include "ace/Containers_T.h"
#include "ace/Vector_T.h"

/**
 * Abstract Syntax Tree Node for an application of an annotation.
 */
class TAO_IDL_FE_Export AST_Annotation_Appl : public virtual AST_Annotation_Decl
{
public:
  /**
   * Store Optional Annotation Parameters
   */
  ///{
  struct Param {
    Identifier *id;
    AST_Expression *expr;
  };
  typedef ACE_Unbounded_Stack<Param*> Params;
  ///}

  /**
   * Create an Annotation using it's name and parameters
   */
  AST_Annotation_Appl (UTL_ScopedName *name, Params *params);

  virtual ~AST_Annotation_Appl ();

  /// Narrowing
  ///{
  DEF_NARROW_FROM_DECL (AST_Annotation_Appl);
  DEF_NARROW_FROM_SCOPE (AST_Annotation_Appl);
  ///}

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Visiting
  virtual int ast_accept (ast_visitor *visitor);

  /// Cleanup
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

  /**
   * Get name of the annoation as written by the user
   */
  const char* original_name () const;

private:
  /// Name of the Annotation as written
  const char *original_name_;

  /// Parameters List
  Params *params_;
};

typedef ACE_Vector<AST_Annotation_Appl*> AST_Annotation_Appls;

struct Decl_Annotations_Pair {
  AST_Decl *decl;
  AST_Annotation_Appls *annotations;
};

#endif
