#ifndef TAO_IDL_AST_FINDER_H
#define TAO_IDL_AST_FINDER_H

// Representation of component home finder construct declaration.

#include "ast_factory.h"

class TAO_IDL_FE_Export AST_Finder : public virtual AST_Factory
{
public:
  AST_Finder (UTL_ScopedName *n);

  ~AST_Finder () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Cleanup function.
  void destroy () override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif // TAO_IDL_AST_FINDER_H
