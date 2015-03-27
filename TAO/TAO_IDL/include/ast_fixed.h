// This may look like C, but it's really -*- C++ -*-
#ifndef AST_FIXED_H
#define AST_FIXED_H

#include "ast_concrete_type.h"

#include "fe_utils.h"

class AST_Expression;

class TAO_IDL_FE_Export AST_Fixed : public virtual AST_ConcreteType
{
public:
  AST_Fixed (UTL_ScopedName *name,
             AST_Expression *digits,
             AST_Expression *scale);

  virtual ~AST_Fixed ();

  DEF_NARROW_FROM_DECL (AST_Fixed);

  virtual void dump (ACE_OSTREAM_TYPE &o);

  virtual void destroy ();

  virtual int ast_accept (ast_visitor *visitor);

  AST_Expression *digits ();
  AST_Expression *scale ();

private:
  AST_Expression *pd_digits;
  AST_Expression *pd_scale;
};

#endif  // AST_FIXED_H
