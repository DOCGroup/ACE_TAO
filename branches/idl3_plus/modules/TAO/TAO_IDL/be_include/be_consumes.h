// -*- C++ -*-
// $Id$

#ifndef BE_CONSUMES_H
#define BE_CONSUMES_H

#include "ast_consumes.h"

#include "be_decl.h"

class be_eventtype;
class be_visitor;

class be_consumes : public virtual AST_Consumes,
                    public virtual be_decl
{
public:
  be_consumes (UTL_ScopedName *n,
               be_eventtype *consumes_type);
            
  virtual ~be_consumes (void);
  
  // Narrowing.
  DEF_NARROW_FROM_DECL(be_consumes);

  // Visiting.
  virtual int accept (be_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);
};

#endif // BE_CONSUMES_H

