// $Id$

#ifndef BE_EXCEPTION_H
#define BE_EXCEPTION_H

#include "be_structure.h"
#include "ast_exception.h"

class be_visitor;

class be_exception : public virtual AST_Exception,
                     public virtual be_structure

{
public:
  be_exception (void);

  be_exception (UTL_ScopedName *n,
                bool local,
                bool abstract);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_exception);
  DEF_NARROW_FROM_SCOPE (be_exception);
};

#endif // end of if !defined (...)
