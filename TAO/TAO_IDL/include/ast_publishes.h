// -*- C++ -*-
#ifndef AST_PUBLISHES_H
#define AST_PUBLISHES_H

#include "ast_field.h"

class AST_EventType;

class TAO_IDL_FE_Export AST_Publishes : public virtual AST_Field
{
public:
  AST_Publishes (UTL_ScopedName *n,
                 AST_Type *publishes_type);

  ~AST_Publishes () override;

  // Data Accessor.
  AST_Type *publishes_type () const;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

  static AST_Decl::NodeType const NT;
};

#endif // AST_PUBLISHES_H

