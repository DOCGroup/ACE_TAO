// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_PORTTYPE_H
#define AST_PORTTYPE_H

#include "ast_decl.h"
#include "ast_template_common.h"
#include "utl_scope.h"

class TAO_IDL_FE_Export AST_PortType
  : public virtual AST_Decl,
    public virtual UTL_Scope,
    public virtual AST_Template_Common
{
public:
  AST_PortType (
    UTL_ScopedName *n,
    const FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_PortType (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_PortType);
  DEF_NARROW_FROM_SCOPE(AST_PortType);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif           // AST_PORTTYPE_H
