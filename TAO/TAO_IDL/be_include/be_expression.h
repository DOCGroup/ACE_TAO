
//=============================================================================
/**
 *  @file    be_expression.h
 *
 *  Extension of class AST_Expression that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_EXPRESSION_H
#define BE_EXPRESSION_H

#include "ast_expression.h"

class be_visitor;

class be_expression : public virtual AST_Expression
{
public:
  // Constructors.
  explicit be_expression (UTL_ScopedName *n);

  be_expression (AST_Expression *b,
                 AST_Expression::ExprType t);

  be_expression (AST_Expression::ExprComb c,
                 AST_Expression *v1,
                 AST_Expression *v2);

  explicit be_expression (ACE_CDR::Long l);

  explicit be_expression (ACE_CDR::LongLong ll);

  explicit be_expression (ACE_CDR::Boolean b);

  explicit be_expression (ACE_CDR::ULong ul);

  explicit be_expression (ACE_CDR::ULongLong ull);

  be_expression (ACE_CDR::ULong l,
                 AST_Expression::ExprType t);

  explicit be_expression (UTL_String *s);

  explicit be_expression (std::string const& s);

  explicit be_expression (ACE_CDR::Char c);

  explicit be_expression (ACE_OutputCDR::from_wchar wc);

  explicit be_expression (ACE_CDR::Double d);

  explicit be_expression (const ACE_CDR::Fixed &f);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif
