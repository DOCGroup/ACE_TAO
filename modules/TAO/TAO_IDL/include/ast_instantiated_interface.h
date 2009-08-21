// -*- C++ -*-
// $Id$

#ifndef AST_INSTANTIATED_INTERFACE_H
#define AST_INSTANTIATED_INTERFACE_H

#include "ast_interface.h"
#include "ast_template_common.h"

class AST_Template_Interface;

class TAO_IDL_FE_Export AST_Instantiated_Interface
  : public virtual AST_Interface
{
public:
  AST_Instantiated_Interface (
    UTL_ScopedName *n,
    AST_Template_Interface *template_ref,
    AST_Template_Common::T_ARGLIST *template_args);

  virtual ~AST_Instantiated_Interface (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Instantiated_Interface);
  DEF_NARROW_FROM_SCOPE (AST_Instantiated_Interface);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  AST_Template_Interface *template_ref_;
  AST_Template_Common::T_ARGLIST *template_args_;
};

#endif /* AST_INSTANTIATED_INTERFACE_H */

