// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_expression.h
//
// = DESCRIPTION
//    Extension of class AST_Expression that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_EXPRESSION_H
#define BE_EXPRESSION_H

#include "ast_expression.h"

class be_visitor;

class be_expression : public virtual AST_Expression
{
public:
  // Constructors.
  be_expression (UTL_ScopedName *n);

  be_expression (AST_Expression *b,
                 AST_Expression::ExprType t);

  be_expression (AST_Expression::ExprComb c,
                 AST_Expression *v1,
                 AST_Expression *v2);

  be_expression (long l);

  be_expression (long l,
                 AST_Expression::ExprType t);

  be_expression (unsigned long l);

  be_expression (UTL_String *s);

  be_expression (char *s);

  be_expression (char c);

  be_expression (ACE_OutputCDR::from_wchar wc);

  be_expression (double d);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif
