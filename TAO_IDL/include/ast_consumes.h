// -*- C++ -*-
// $Id$

#ifndef AST_CONSUMES_H
#define AST_CONSUMES_H

#include "ast_field.h"

class AST_EventType;

class TAO_IDL_FE_Export AST_Consumes : public virtual AST_Field
{
public:
  AST_Consumes (UTL_ScopedName *n,
                AST_Type *consumes_type);

  virtual ~AST_Consumes (void);

  // Data Accessor.
  AST_Type *consumes_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Consumes);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  static AST_Decl::NodeType const NT;
};

#endif // AST_CONSUMES_H

