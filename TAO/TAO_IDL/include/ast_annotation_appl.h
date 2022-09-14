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
 *
 * Params are implemented as a stack of name, value pairs which are independent
 * from the finished annotation members. The members which are AST_Fields subclasses
 * stored the same way as fields in AST_Struct.
 */
class TAO_IDL_FE_Export AST_Annotation_Appl : public virtual AST_Annotation_Decl
{
public:
  /**
   * Store Optional Annotation Parameters
   */
  ///{
  struct Param {
    Param ();
    ~Param ();
    Identifier *id;
    AST_Expression *expr;
    bool used;
    typedef ACE_Unbounded_Stack_Iterator<Param*> Iterator;
  };
  typedef ACE_Unbounded_Stack<Param*> Params;
  ///}

  /// Delete Annotation Parameters
  static void delete_params (Params* params);

  /**
   * Create an Annotation using it's name and parameters
   */
  AST_Annotation_Appl (UTL_ScopedName *name, Params *params);

  virtual ~AST_Annotation_Appl ();

  /// AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  /// Visiting
  virtual int ast_accept (ast_visitor *visitor);

  /// Cleanup
  virtual void destroy ();

  static AST_Decl::NodeType const NT;

  /**
   * Get name of the annotation as written by the user
   */
  const char* original_name () const;

  /**
   * Apply a Annotation Declaration to this Application. This will either fully
   * instantiate this object or result in an error, in which case it returns false.
   */
  bool apply_from (AST_Annotation_Decl *decl);

  /**
   * Return pointer to the stack of parameters passed. BACKENDS SHOULDN'T USE
   * THIS UNLESS YOU NEED EXACTLY WHAT THE USER PASSED. Backends should use the
   * lookup and scope iterator APIs that this class inherits from AST_Scope
   * and AST_Struct. Can be null.
   */
  Params *params ();

  /**
   * AST_Annotation_Decl kept in case desired by someone
   *
   * Will be null if accessed before apply_from() is called or if being used
   * without a declaration.
   */
  AST_Annotation_Decl *annotation_decl ();

  /**
   * Return the parameter with the specified name if it's in the stack else
   * returns 0.
   */
  Param *find_param (const char *name);

private:
  /// Name of the Annotation as written
  const char *original_name_;

  /// Parameters List (Can be null)
  Params *params_;

  /// The AST_Annotation_Decl (Possibly could be null)
  AST_Annotation_Decl *annotation_decl_;
};

struct Decl_Annotations_Pair {
  AST_Decl *decl;
  AST_Annotation_Appls *annotations;
};

#endif
