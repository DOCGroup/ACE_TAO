// -*- C++ -*-
#ifndef AST_CONSUMES_H
#define AST_CONSUMES_H

#include "ast_field.h"

class AST_EventType;

class TAO_IDL_FE_Export AST_Consumes : public virtual AST_Field
{
public:
  AST_Consumes (UTL_ScopedName *n,
                AST_Type *consumes_type);

  ~AST_Consumes () override;

  // Data Accessor.
  AST_Type *consumes_type () const;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

  static AST_Decl::NodeType const NT;
};

#endif // AST_CONSUMES_H

