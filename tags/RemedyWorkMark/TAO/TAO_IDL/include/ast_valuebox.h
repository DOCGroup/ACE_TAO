// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_VALUEBOX_AST_VALUEBOX_HH
#define _AST_VALUEBOX_AST_VALUEBOX_HH

#include "ast_concrete_type.h"

class TAO_IDL_FE_Export AST_ValueBox :  public virtual AST_ConcreteType
{
public:
  AST_ValueBox (UTL_ScopedName *n,
                AST_Type       *boxed_type);

  virtual ~AST_ValueBox (void);

  // Data Accessors.
  AST_Type            *boxed_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_ValueBox);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  static AST_Decl::NodeType const NT;

protected:
  AST_Type                    *pd_boxed_type;
};

#endif           // _AST_VALUEBOX_AST_VALUEBOX_HH
