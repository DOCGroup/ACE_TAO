#ifndef _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH
#define _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH

#include "ast_interface_fwd.h"

// Representation of a forward valuetype declaration.

class TAO_IDL_FE_Export AST_ValueTypeFwd : public virtual AST_InterfaceFwd
{
public:
  AST_ValueTypeFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  ~AST_ValueTypeFwd () override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH
