// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_PORTTYPE_H
#define AST_PORTTYPE_H

#include "ast_decl.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_PortType : public virtual AST_Decl
{
public:
  AST_PortType (
    UTL_ScopedName *n,
    const FE_Utils::T_PARAMLIST_INFO *template_params);
    
  virtual ~AST_PortType (void);

  FE_Utils::T_PARAMLIST_INFO &template_params (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_PortType);
  DEF_NARROW_FROM_SCOPE(AST_PortType);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  FE_Utils::T_PARAMLIST_INFO template_params_;
};

#endif           // AST_PORTTYPE_H
