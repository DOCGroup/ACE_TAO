// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_MODULE_H
#define AST_TEMPLATE_MODULE_H

#include "ast_module.h"
#include "ast_type.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Template_Module
  : public virtual AST_Module,
    public virtual AST_Type
{
public:
  AST_Template_Module (UTL_ScopedName *n,
                       FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Template_Module (void);

  FE_Utils::T_PARAMLIST_INFO const *
  template_params (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL (AST_Template_Module);
  DEF_NARROW_FROM_SCOPE (AST_Template_Module);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  FE_Utils::T_PARAMLIST_INFO * template_params_;
};

#endif           // AST_TEMPLATE_MODULE_H
