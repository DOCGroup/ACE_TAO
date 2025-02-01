#ifndef BE_EXCEPTION_H
#define BE_EXCEPTION_H

#include "be_structure.h"
#include "ast_exception.h"

class be_visitor;

class be_exception : public virtual AST_Exception,
                     public virtual be_structure

{
public:
  be_exception (UTL_ScopedName *n,
                bool local,
                bool abstract);

  // Cleanup function.
  virtual void destroy ();

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif // end of if !defined (...)
