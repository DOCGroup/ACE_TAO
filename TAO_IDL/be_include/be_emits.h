// -*- C++ -*-
// $Id$

#ifndef BE_EMITS_H
#define BE_EMITS_H

#include "ast_emits.h"

#include "be_field.h"

class be_eventtype;
class be_visitor;

class be_emits : public virtual AST_Emits,
                 public virtual be_field
{
public:
  be_emits (UTL_ScopedName *n,
            AST_Type *emits_type);

  virtual ~be_emits (void);

  // Non-virtual override of frontend method.
  be_eventtype *emits_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_emits);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);
};

#endif // BE_EMITS_H

