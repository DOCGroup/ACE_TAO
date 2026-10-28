#ifndef _AST_NATIVE_H_
#define _AST_NATIVE_H_

#include "ast_exception.h"

// Representation of "native" IDL type. It may be used as a
// return type, parameter type, or in an operation's
// exception list. This last usage creates special problems
// with both syntax checking and code generation. Letting
// this class inherit from AST_Exception is the most seamless
// way to handle it, and does not affect the other use cases.
class TAO_IDL_FE_Export AST_Native : public virtual AST_Exception
{
public:
  AST_Native (UTL_ScopedName *n);

  ~AST_Native () override;

  // Cleanup.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif /* AST_NATIVE_H */
