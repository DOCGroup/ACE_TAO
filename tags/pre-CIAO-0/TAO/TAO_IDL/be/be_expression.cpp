// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_expression.cpp
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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_expression, "$Id$")

be_expression::be_expression (UTL_ScopedName *n)
  : AST_Expression (n)
{
}

be_expression::be_expression (AST_Expression *b, AST_Expression::ExprType t)
  : AST_Expression (b,
                    t)
{
}

be_expression::be_expression (AST_Expression::ExprComb c,
                              AST_Expression *v1,
                              AST_Expression *v2)
  : AST_Expression (c,
                    v1,
                    v2)
{
}

be_expression::be_expression (long l)
  : AST_Expression (l)
{
}

be_expression::be_expression (long l,
                              AST_Expression::ExprType t)
  : AST_Expression (l,
                    t)
{
}

be_expression::be_expression (unsigned long l)
  : AST_Expression (l)
{
}

be_expression::be_expression (UTL_String *s)
  : AST_Expression (s)
{
}

be_expression::be_expression (char *s)
  : AST_Expression (s)
{
}

be_expression::be_expression (char c)
  : AST_Expression (c)
{
}

be_expression::be_expression (ACE_OutputCDR::from_wchar wc)
  : AST_Expression (wc)
{
}

be_expression::be_expression (double d)
  : AST_Expression (d)
{
}

int
be_expression::accept (be_visitor *visitor)
{
  return visitor->visit_expression (this);
}
