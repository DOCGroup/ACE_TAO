#ifndef _AST_NATIVE_H_
#define _AST_NATIVE_H_

#include "ast_exception.h"

/**
 * @brief Represents an IDL native type.
 *
 * A native type may appear as a return type, parameter type or in an
 * operation's exception list. Inheriting from AST_Exception supports
 * the latter use in syntax checking and code generation.
 */
class TAO_IDL_FE_Export AST_Native : public virtual AST_Exception
{
public:
  AST_Native () = delete;
  explicit AST_Native (UTL_ScopedName *n);

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
