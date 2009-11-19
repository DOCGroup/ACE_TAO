// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_PARAM_HOLDER_H
#define AST_PARAM_HOLDER_H

#include "ast_decl.h"

class Identifier;

class TAO_IDL_FE_Export AST_Param_Holder
  : public virtual AST_Decl
{
public:
  AST_Param_Holder (Identifier *parameter_name);

  virtual ~AST_Param_Holder (void);
  
  Identifier const * const param_name (void) const;
  
  // Narrowing.
  DEF_NARROW_FROM_DECL (AST_Param_Holder);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);
  
  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
  
private:
  Identifier *param_name_;
};

#endif // AST_PARAM_HOLDER_H