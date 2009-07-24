// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_COMMON_H
#define AST_TEMPLATE_COMMON_H

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Template_Common
{
public:
  AST_Template_Common (FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Template_Common (void);

  FE_Utils::T_PARAMLIST_INFO *template_params (void);
  
  virtual void destroy (void);

protected:
  FE_Utils::T_PARAMLIST_INFO * template_params_;
};

#endif // AST_TEMPLATE_COMMON_H

