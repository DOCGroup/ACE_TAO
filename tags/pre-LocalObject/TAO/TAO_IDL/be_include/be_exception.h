// $Id$

#ifndef BE_EXCEPTION_H
#define BE_EXCEPTION_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_exception.h"

class UTL_StrList;
class be_visitor;

/*
 * BE_Exception
 */
class be_exception : public virtual AST_Exception,
                     public virtual be_scope,
                     public virtual be_type

{
public:
  // Operations
  be_exception (void);
  // default constructor

  be_exception (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  // =code generation

  virtual int member_count (void);
  // return the count of members

  virtual idl_bool in_recursion (be_type *node = 0);
  // check if we or the parameter node is in recursion

  virtual int gen_iostream_op_hdr (TAO_OutStream *os);
  // Generate declaration of iostream operator(s).

  virtual int gen_iostream_op_impl (TAO_OutStream *os);
  // Generate definition of iostream operator(s).

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_exception);
  DEF_NARROW_FROM_SCOPE (be_exception);

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int member_count_;
  // number of members
};

#endif // end of if !defined (...)
