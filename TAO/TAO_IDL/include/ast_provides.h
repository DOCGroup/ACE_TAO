// -*- C++ -*-
#ifndef AST_PROVIDES_H
#define AST_PROVIDES_H

#include "ast_field.h"

class TAO_IDL_FE_Export AST_Provides : public virtual AST_Field
{
public:
  AST_Provides (UTL_ScopedName *n,
                AST_Type *provides_type);

  ~AST_Provides () override;

  // Data Accessor.
  AST_Type *provides_type () const;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

  static AST_Decl::NodeType const NT;
};

#endif // AST_PROVIDES_H

