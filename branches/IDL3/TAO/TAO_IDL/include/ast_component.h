// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_INTERFACE_AST_COMPONENT_HH
#define _AST_INTERFACE_AST_COMPONENT_HH

#include "ast_interface.h"

class TAO_IDL_FE_Export AST_Component : public virtual AST_Interface
{
public:
  AST_Component (void);

  AST_Component (UTL_ScopedName *n,
                 AST_Component *parent,
                 AST_Interface **supports,
                 long n_supports,
                 AST_Interface **supports_flat,
                 long n_supports_flat);

  virtual ~AST_Component (void);

  AST_Component *parent (void);

private:
  AST_Component *parent_;
};

#endif // _AST_INTERFACE_AST_COMPONENT_HH
