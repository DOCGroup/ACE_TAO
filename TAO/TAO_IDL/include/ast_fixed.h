#ifndef AST_FIXED_H
#define AST_FIXED_H

#include "ast_concrete_type.h"

#include "fe_utils.h"

#include <memory>

class AST_Expression;

class TAO_IDL_FE_Export AST_Fixed : public virtual AST_ConcreteType
{
public:
  AST_Fixed (UTL_ScopedName *name,
             AST_Expression *digits,
             AST_Expression *scale);
  AST_Fixed (AST_Fixed const &) = delete;
  AST_Fixed (AST_Fixed &&) = delete;
  AST_Fixed &operator= (AST_Fixed const &) = delete;
  AST_Fixed &operator= (AST_Fixed &&) = delete;

  ~AST_Fixed () override;

  void dump (ACE_OSTREAM_TYPE &o) override;

  void destroy () override;

  int ast_accept (ast_visitor *visitor) override;

  AST_Expression *digits () const;
  AST_Expression *scale () const;

private:
  std::unique_ptr<AST_Expression> pd_digits;
  std::unique_ptr<AST_Expression> pd_scale;
};

#endif  // AST_FIXED_H
