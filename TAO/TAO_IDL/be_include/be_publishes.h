// -*- C++ -*-
#ifndef BE_PUBLISHES_H
#define BE_PUBLISHES_H

#include "ast_publishes.h"

#include "be_field.h"

class be_eventtype;
class be_visitor;

class be_publishes : public virtual AST_Publishes,
                     public virtual be_field
{
public:
  be_publishes (UTL_ScopedName *n,
                AST_Type *publishes_type);

  virtual ~be_publishes ();

  // Non-virtual override of frontend method.
  be_eventtype *publishes_type () const;

  // Visiting.
  int accept (be_visitor *visitor) override;

  // Cleanup.
  void destroy () override;
};

#endif // BE_PUBLISHES_H

