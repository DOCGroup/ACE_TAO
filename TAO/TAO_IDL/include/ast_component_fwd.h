#ifndef _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH
#define _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH

#include "ast_interface_fwd.h"

// Representation of a forward component declaration.

class TAO_IDL_FE_Export AST_ComponentFwd : public virtual AST_InterfaceFwd
{
public:
  AST_ComponentFwd (AST_ComponentFwd const &) = delete;
  AST_ComponentFwd (AST_ComponentFwd &&) = delete;
  AST_ComponentFwd &operator= (AST_ComponentFwd const &) = delete;
  AST_ComponentFwd &operator= (AST_ComponentFwd &&) = delete;
  AST_ComponentFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  ~AST_ComponentFwd () override = default;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Is this decl a forward declared type (Yes)
  bool is_fwd () const override;

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH
