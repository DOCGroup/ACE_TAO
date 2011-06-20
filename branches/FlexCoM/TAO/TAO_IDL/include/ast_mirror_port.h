// -*- C++ -*-
// $Id$

#ifndef AST_MIRROR_PORT_H
#define AST_MIRROR_PORT_H

#include "ast_extended_port.h"

class TAO_IDL_FE_Export AST_Mirror_Port
  : public virtual AST_Extended_Port
{
public:
  AST_Mirror_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual ~AST_Mirror_Port (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Mirror_Port);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
};

#endif /* AST_MIRROR_PORT_H */

