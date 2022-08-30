#ifndef _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
#define _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH

#include "ast_valuetype_fwd.h"

// Representation of a forward eventtype declaration.

class TAO_IDL_FE_Export AST_EventTypeFwd : public virtual AST_ValueTypeFwd
{
public:
  AST_EventTypeFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  ~AST_EventTypeFwd () override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
