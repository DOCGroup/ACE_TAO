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

  virtual ~AST_Emits ();

  // Data Accessor.
  AST_Type *emits_type () const;

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy ();

  static AST_Decl::NodeType const NT;
};

#endif // AST_EMITS_H

