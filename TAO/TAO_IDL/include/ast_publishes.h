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

  virtual ~AST_Publishes ();

  // Data Accessor.
  AST_Type *publishes_type () const;

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy ();

  static AST_Decl::NodeType const NT;
};

#endif // AST_PUBLISHES_H

