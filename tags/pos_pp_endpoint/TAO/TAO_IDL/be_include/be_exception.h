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

  be_exception (UTL_ScopedName *n,
                UTL_StrList *p,
                idl_bool local,
                idl_bool abstract);
  // constructor

  // =code generation

  virtual idl_bool in_recursion (be_type *node = 0);
  // check if we or the parameter node is in recursion

  virtual void destroy (void);
  // Cleanup function.

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_exception);
  DEF_NARROW_FROM_SCOPE (be_exception);
};

#endif // end of if !defined (...)
