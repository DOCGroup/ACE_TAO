// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_INTERFACE_H
#define AST_TEMPLATE_INTERFACE_H

#include "ast_interface.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Template_Interface
  : public virtual AST_Interface
{
public:
  AST_Template_Interface (
    UTL_ScopedName *n,
    AST_Interface **ih,
    long nih,
    AST_Interface **ih_flat,
    long nih_flat,
    const FE_Utils::T_PARAMLIST_INFO &template_params);

  virtual ~AST_Template_Interface (void);

  virtual void destroy (void);

protected:
  FE_Utils::T_PARAMLIST_INFO template_params_;
};

#endif /* AST_TEMPLATE_INTERFACE_H */

