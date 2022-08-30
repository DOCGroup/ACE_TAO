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
  void destroy () override;

  // Visiting.
  int accept (be_visitor *visitor) override;
};

#endif // end of if !defined (...)
