// -*- C++ -*-
// $Id$

#ifndef AST_PROVIDES_H
#define AST_PROVIDES_H

#include "ast_field.h"

class TAO_IDL_FE_Export AST_Provides : public virtual AST_Field
{
public:
  AST_Provides (UTL_ScopedName *n,
                AST_Type *provides_type);

  virtual ~AST_Provides (void);

  // Data Accessor.
  AST_Type *provides_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Provides);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  static AST_Decl::NodeType const NT;
};

#endif // AST_PROVIDES_H

