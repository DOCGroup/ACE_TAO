// -*- C++ -*-
// $Id$

#ifndef AST_EXTENDED_PORT_H
#define AST_EXTENDED_PORT_H

#include "ast_field.h"
#include "ast_porttype.h"

class TAO_IDL_FE_Export AST_Extended_Port
  : public virtual AST_Field
{
public:
  AST_Extended_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual ~AST_Extended_Port (void);

  AST_PortType *port_type (void) const;

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Extended_Port);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
};

#endif /* AST_EXTENDED_PORT_H */

