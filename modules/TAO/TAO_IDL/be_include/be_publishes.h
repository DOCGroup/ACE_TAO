// -*- C++ -*-
// $Id$

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
                AST_EventType *publishes_type);
            
  virtual ~be_publishes (void);
  
  // Narrowing.
  DEF_NARROW_FROM_DECL(be_publishes);

  // Visiting.
  virtual int accept (be_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);
};

#endif // BE_PUBLISHES_H

