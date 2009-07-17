// -*- C++ -*-
// $Id$

#ifndef BE_PROVIDES_H
#define BE_PROVIDES_H

#include "ast_provides.h"

#include "be_decl.h"

class be_interface;
class be_visitor;

class be_provides : public virtual AST_Provides,
                    public virtual be_decl
{
public:
  be_provides (UTL_ScopedName *n,
               AST_Interface *provides_type);
            
  virtual ~be_provides (void);
  
  // Narrowing.
  DEF_NARROW_FROM_DECL(be_provides);

  // Visiting.
  virtual int accept (be_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);
};

#endif // BE_PROVIDES_H

