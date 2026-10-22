// -*- C++ -*-
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

  ~AST_Mirror_Port () override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif /* AST_MIRROR_PORT_H */

