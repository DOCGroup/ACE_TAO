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

#include "be_constant.h"
#include "be_visitor.h"
#include "utl_scope.h"
#include "nr_extern.h"

ACE_RCSID (be, 
           be_constant, 
           "$Id$")

be_constant::be_constant (void)
{
}

be_constant::be_constant (AST_Expression::ExprType et,
			                    AST_Expression *v,
                          UTL_ScopedName *n)
  : AST_Constant (et,
                  v,
                  n),
    AST_Decl (AST_Decl::NT_const,
              n)
{
}

const char *
be_constant::exprtype_to_string (void)
{
  switch (this->pd_et)
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
    case AST_Expression::EV_void:
      return "void";
    case AST_Expression::EV_none:
      return "none";
    case AST_Expression::EV_longlong:
      return "CORBA::LongLong";
    case AST_Expression::EV_ulonglong:
      return "CORBA::ULongLong";
    case AST_Expression::EV_wchar:
      return "CORBA::WChar";
    case AST_Expression::EV_wstring:
      return "CORBA::WChar *const";
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_enum:
      return 0;
    }

  return 0;
}

UTL_ScopedName *
be_constant::enum_full_name (void)
{
  if (this->pd_et == AST_Expression::EV_enum)
    {
      UTL_Scope *s = this->defined_in ();
      AST_Decl *d = s->lookup_by_name (this->pd_constant_value->n (),
                                       1);
      return (ScopeAsDecl (d->defined_in ()))->name ();
    }
  else
    {
      return 0;
    }
}

int
be_constant::accept (be_visitor *visitor)
{
  return visitor->visit_constant (this);
}

void
be_constant::destroy (void)
{
  this->AST_Constant::destroy ();
  this->be_decl::destroy ();
}

// Narrowing
IMPL_NARROW_METHODS2 (be_constant, AST_Constant, be_decl)
IMPL_NARROW_FROM_DECL (be_constant)
