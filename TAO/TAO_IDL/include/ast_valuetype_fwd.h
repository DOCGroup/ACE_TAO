#ifndef _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH
#define _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH

#include "ast_interface_fwd.h"

// Representation of a forward valuetype declaration.

class TAO_IDL_FE_Export AST_ValueTypeFwd : public virtual AST_InterfaceFwd
{
public:
  AST_ValueTypeFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  virtual ~AST_ValueTypeFwd ();

  // Cleanup function.
  virtual void destroy ();

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_INTERFACE_FWD_AST_VALUETYPE_FWD_HH
