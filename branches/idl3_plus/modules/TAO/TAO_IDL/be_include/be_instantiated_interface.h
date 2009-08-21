// -*- C++ -*-
// $Id$

#ifndef BE_INSTANTIATED_INTERFACE_H
#define BE_INSTANTIATED_INTERFACE_H

#include "ast_instantiated_interface.h"

class be_visitor;

class be_instantiated_interface
  : public virtual AST_Instantiated_Interface
{
public:
  be_instantiated_interface (
    UTL_ScopedName *n,
    AST_Template_Interface *template_ref,
    AST_Template_Common::T_ARGLIST *template_args);

  virtual ~be_instantiated_interface (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_instantiated_interface);
  DEF_NARROW_FROM_SCOPE (be_instantiated_interface);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif /* BE_INSTANTIATED_INTERFACE_H */

