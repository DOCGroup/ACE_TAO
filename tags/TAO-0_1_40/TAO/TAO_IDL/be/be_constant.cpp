// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_constant.cpp
//
// = DESCRIPTION
//    Extension of class AST_Constant that provides additional means for C++
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

/*
 * BE_Constant
 */
be_constant::be_constant (void)
{
  this->size_type (be_decl::FIXED);
}

be_constant::be_constant (AST_Expression::ExprType et,
			  AST_Expression *v,
                          UTL_ScopedName *n,
                          UTL_StrList *p)
  : AST_Constant (et, v, n, p),
    AST_Decl (AST_Decl::NT_const, n, p)
{
  this->size_type (be_decl::FIXED);
}

char *
be_constant::exprtype_to_string (void)
{
  switch (this->et ())
    {
    case AST_Expression::EV_short:
      return "CORBA::Short";
    case AST_Expression::EV_ushort:
      return "CORBA::UShort";
    case AST_Expression::EV_long:
      return "CORBA::Long";
    case AST_Expression::EV_ulong:
      return "CORBA::ULong";
    case AST_Expression::EV_float:
      return "CORBA::Float";
    case AST_Expression::EV_double:
      return "CORBA::Double";
    case AST_Expression::EV_char:
      return "CORBA::Char";
    case AST_Expression::EV_octet:
      return "CORBA::Octet";
    case AST_Expression::EV_bool:
      return "CORBA::Boolean";
    case AST_Expression::EV_string:
      return "char *const";
    case AST_Expression::EV_any:
      return "CORBA::Any";
    case AST_Expression::EV_void:
      return "void";
    case AST_Expression::EV_none:
      return "none";
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wchar:
    case AST_Expression::EV_wstring:
      return NULL;
    }
  return NULL;
}

int
be_constant::accept (be_visitor *visitor)
{
  return visitor->visit_constant (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_constant, AST_Constant, be_decl)
IMPL_NARROW_FROM_DECL (be_constant)
