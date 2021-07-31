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

  virtual ~AST_Consumes ();

  // Data Accessor.
  AST_Type *consumes_type () const;

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy ();

  static AST_Decl::NodeType const NT;
};

#endif // AST_CONSUMES_H

