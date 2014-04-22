// -*- C++ -*-
// $Id$

#ifndef AST_USES_H
#define AST_USES_H

#include "ast_field.h"

class TAO_IDL_FE_Export AST_Uses : public virtual AST_Field
{
public:
  AST_Uses (UTL_ScopedName *n,
            AST_Type *uses_type,
            bool is_multiple);

  virtual ~AST_Uses (void);

  // Data Accessors.
  AST_Type *uses_type (void) const;
  bool is_multiple (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Uses);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  static AST_Decl::NodeType const NT;

private:
  bool is_multiple_;
};

#endif // AST_USES_H

