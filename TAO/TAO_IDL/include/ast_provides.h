// -*- C++ -*-
#ifndef AST_PROVIDES_H
#define AST_PROVIDES_H

#include "ast_field.h"

class TAO_IDL_FE_Export AST_Provides : public virtual AST_Field
{
public:
  AST_Provides (UTL_ScopedName *n,
                AST_Type *provides_type);

  virtual ~AST_Provides ();

  // Data Accessor.
  AST_Type *provides_type () const;

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy ();

  static AST_Decl::NodeType const NT;
};

#endif // AST_PROVIDES_H

