// -*- C++ -*-
#ifndef AST_EMITS_H
#define AST_EMITS_H

#include "ast_field.h"

class AST_EventType;

class TAO_IDL_FE_Export AST_Emits : public virtual AST_Field
{
public:
  AST_Emits (UTL_ScopedName *n,
             AST_Type *emits_type);

  ~AST_Emits () override;

  // Data Accessor.
  AST_Type *emits_type () const;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

  static AST_Decl::NodeType const NT;
};

#endif // AST_EMITS_H

