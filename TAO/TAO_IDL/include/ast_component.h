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

  // Accessors.

  AST_Component *parent (void) const;
  AST_Interface **supports (void) const;
  long n_supports (void) const;
  AST_Interface **supports_flat (void) const;
  long n_supports_flat (void) const;

private:
  AST_Component *pd_parent;
  AST_Interface **pd_supports;
  long pd_n_supports;
  AST_Interface **pd_supports_flat;
  long pd_n_supports_flat;
};

#endif // _AST_INTERFACE_AST_COMPONENT_HH
