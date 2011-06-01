// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

// AST_Constant nodes denote IDL constant declarations.
// AST_Constants have a value (an AST_Expression) and a value type
// (a value from the enum AST_Expression::ExprType).
// AST_Constant has two constructors, one for use in creating constants
// and the other for use in creating enumerators (see the class
// AST_EnumVal).

#include "ast_constant.h"
#include "utl_identifier.h"
#include "ast_visitor.h"
#include "ast_generator.h"
#include "nr_extern.h"

// Static functions.

// Convert a value from the enum AST_Expression::ExprType to a char *.
const char *
AST_Constant::exprtype_to_string (AST_Expression::ExprType et)
{
  switch (et)
    {
    case AST_Expression::EV_short:
      return "Short";
    case AST_Expression::EV_ushort:
      return "UShort";
    case AST_Expression::EV_long:
      return "Long";
    case AST_Expression::EV_ulong:
      return "ULong";
    case AST_Expression::EV_float:
      return "Float";
    case AST_Expression::EV_double:
      return "Double";
    case AST_Expression::EV_char:
      return "Char";
    case AST_Expression::EV_octet:
      return "Octet";
    case AST_Expression::EV_bool:
      return "Boolean";
    case AST_Expression::EV_string:
      return "Char*";
    case AST_Expression::EV_ulonglong:
      return "ULongLong";
    case AST_Expression::EV_longlong:
      return "LongLong";
    case AST_Expression::EV_wchar:
      return "Wchar";
    case AST_Expression::EV_wstring:
      return "Wchar*";
    case AST_Expression::EV_longdouble:
      return "LongDouble";
    default:
      break;
    }

  return 0;
}

AST_Decl::NodeType const
AST_Constant::NT = AST_Decl::NT_const;

// Used in constructing AST_EnumVal nodes.
AST_Constant::AST_Constant (AST_Expression::ExprType t,
                            AST_Decl::NodeType nt,
                            AST_Expression *v,
                            UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (nt,
              n),
    pd_constant_value (v),
    pd_et (t),
    ifr_added_ (0)
{
}

// Used when constructing AST_Constant nodes.
AST_Constant::AST_Constant (AST_Expression::ExprType t,
                            AST_Expression *v,
                            UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_const,
              n),
    pd_constant_value (v),
    pd_et (t),
    ifr_added_ (0)
{
  // Avoids a truncation warning on MSVC when assigning a decimal
  // literal to a float constant. Must also check that the input
  // expression is of type double (indicates that we are being
  // assigned from a literal). If v is of type float, it may be
  // a constant-to-constant assignment - in any case the danger
  // of truncation would not apply.
  if (t == AST_Expression::EV_float && v->ev ()->et == AST_Expression::EV_double)
    {
      AST_Expression::AST_ExprValue *ev =
        this->pd_constant_value->ev ();
      ev->et = t;
      ev->u.fval = (float) ev->u.dval;
    }
  // Allows the enum value string name to be used in generating the
  // rhs of the constant assignment.
  else if (t == AST_Expression::EV_enum)
    {
      this->pd_constant_value->ev ()->et = t;
    }
}

AST_Constant::~AST_Constant (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_Constant node to the ostream o.
void
AST_Constant::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "const ");
  this->dump_i (o, this->exprtype_to_string ());
  this->dump_i (o, " ");

  this->local_name ()->dump (o);

  this->dump_i (o, " = ");

  this->pd_constant_value->dump (o);
}

int
AST_Constant::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_constant (this);
}

void
AST_Constant::destroy (void)
{
  if (this->pd_constant_value != 0)
    {
      this->pd_constant_value->destroy ();
      delete this->pd_constant_value;
      this->pd_constant_value = 0;
    }

  this->AST_Decl::destroy ();
}

// Data accessors.

AST_Expression *
AST_Constant::constant_value (void)
{
  return this->pd_constant_value;
}

AST_Expression::ExprType
AST_Constant::et (void)
{
  return this->pd_et;
}

bool
AST_Constant::ifr_added (void)
{
  return this->ifr_added_;
}

void
AST_Constant::ifr_added (bool val)
{
  this->ifr_added_ = val;
}

const char *
AST_Constant::exprtype_to_string (void)
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
    case AST_Expression::EV_longdouble:
      return "CORBA::LongDouble";
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
    case AST_Expression::EV_enum:
    case AST_Expression::EV_any:
    case AST_Expression::EV_object:
      return 0;
    }

  return 0;
}

UTL_ScopedName *
AST_Constant::enum_full_name (void)
{
  if (this->pd_et == AST_Expression::EV_enum)
    {
      UTL_Scope * const s = this->defined_in ();
      AST_Decl  * const d = s->lookup_by_name (this->pd_constant_value->n (),
                                               1);
      return (d ? (ScopeAsDecl (d->defined_in ()))->name () : 0);
    }
  else
    {
      return 0;
    }
}

IMPL_NARROW_FROM_DECL(AST_Constant)
