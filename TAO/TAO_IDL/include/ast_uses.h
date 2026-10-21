// -*- C++ -*-
#ifndef AST_USES_H
#define AST_USES_H

#include "ast_field.h"

class TAO_IDL_FE_Export AST_Uses : public virtual AST_Field
{
public:
  AST_Uses (UTL_ScopedName *n,
            AST_Type *uses_type,
            bool is_multiple);

  ~AST_Uses () override;

  // Data Accessors.
  AST_Type *uses_type () const;
  bool is_multiple () const;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

  static AST_Decl::NodeType const NT;

private:
  bool is_multiple_;
};

#endif // AST_USES_H

