// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_PARAM_HOLDER_H
#define AST_PARAM_HOLDER_H

#include "ast_type.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Param_Holder
  : public virtual AST_Type
{
public:
  AST_Param_Holder (UTL_ScopedName *parameter_name,
                    FE_Utils::T_Param_Info *info);

  virtual ~AST_Param_Holder (void);

  FE_Utils::T_Param_Info const *info (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL (AST_Param_Holder);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

private:
  FE_Utils::T_Param_Info *info_;
};

#endif // AST_PARAM_HOLDER_H

