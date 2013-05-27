// -*- C++ -*-
// $Id$

#ifndef BE_USES_H
#define BE_USES_H

#include "ast_uses.h"

#include "be_field.h"

class be_interface;
class be_visitor;

class be_uses : public virtual AST_Uses,
                public virtual be_field
{
public:
  be_uses (UTL_ScopedName *n,
           AST_Type *uses_type,
           bool is_multiple);

  virtual ~be_uses (void);

  // Non-virtual override of frontend method.
  be_type *uses_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(be_uses);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  void original_uses (be_uses *original_uses);
  // Sets the original uses from which this one was created,
  // applies only to implied IDL.

  be_uses *original_uses ();
  // Returns the original uses from which this one was created,
  // applies only to implied IDL

private:
  be_uses *original_uses_;
};

#endif // BE_USES_H

