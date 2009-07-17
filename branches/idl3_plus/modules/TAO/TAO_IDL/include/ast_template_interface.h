// -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_INTERFACE_H
#define AST_TEMPLATE_INTERFACE_H

#include "ast_interface.h"
#include "ast_template_common.h"

class TAO_IDL_FE_Export AST_Template_Interface
  : public virtual AST_Interface,
    public virtual AST_Template_Common
{
public:
  AST_Template_Interface (
    UTL_ScopedName *n,
    AST_Interface **ih,
    long nih,
    AST_Interface **ih_flat,
    long nih_flat,
    const FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Template_Interface (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Template_Interface);
  DEF_NARROW_FROM_SCOPE(AST_Template_Interface);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  FE_Utils::T_PARAMLIST_INFO template_params_;
};

#endif /* AST_TEMPLATE_INTERFACE_H */

