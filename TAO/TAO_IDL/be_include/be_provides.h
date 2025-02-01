// -*- C++ -*-
#ifndef BE_PROVIDES_H
#define BE_PROVIDES_H

#include "ast_provides.h"

#include "be_field.h"
#include "be_interface.h"

class be_interface;
class be_visitor;

class be_provides : public virtual AST_Provides,
                    public virtual be_field
{
public:
  be_provides (UTL_ScopedName *n,
               AST_Type *provides_type);

  virtual ~be_provides ();

  // Non-virtual override of frontend method.
  be_type *provides_type () const;

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy ();
};

#endif // BE_PROVIDES_H

