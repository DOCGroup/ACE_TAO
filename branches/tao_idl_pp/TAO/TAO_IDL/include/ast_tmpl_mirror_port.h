// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TMPL_MIRROR_PORT_H
#define AST_TMPL_MIRROR_PORT_H

#include "ast_tmpl_port.h"

class TAO_IDL_FE_Export AST_Tmpl_Mirror_Port
  : public virtual AST_Tmpl_Port
{
public:
  AST_Tmpl_Mirror_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual ~AST_Tmpl_Mirror_Port (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Tmpl_Mirror_Port);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif /* AST_TMPL_MIRROR_PORT_H */

