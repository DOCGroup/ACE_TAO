// -*- C++ -*-
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

  ~AST_Extended_Port () override;

  AST_PortType *port_type () const;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif /* AST_EXTENDED_PORT_H */

