// $Id$

#ifndef BE_EXCEPTION_H
#define BE_EXCEPTION_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_exception.h"

class be_visitor;

class be_exception : public virtual AST_Exception,
                     public virtual be_scope,
                     public virtual be_type

{
public:
  be_exception (void);
  // Default constructor.

  be_exception (UTL_ScopedName *n,
                idl_bool local,
                idl_bool abstract);
  // Constructor.

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_exception);
  DEF_NARROW_FROM_SCOPE (be_exception);
};

#endif // end of if !defined (...)
