#ifndef BE_FIXED_H
#define BE_FIXED_H

#include "ast_fixed.h"
#include "be_type.h"

class AST_Expression;
class be_visitor;

class be_fixed : public virtual AST_Fixed, public virtual be_type
{
public:
  be_fixed (UTL_ScopedName *name,
            AST_Expression *digits,
            AST_Expression *scale);

  virtual int accept (be_visitor *visitor);

  virtual void destroy ();

  DEF_NARROW_FROM_DECL (be_fixed);
};

#endif
