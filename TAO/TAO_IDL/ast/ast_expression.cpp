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

// AST_Expression nodes denote IDL expressions used in the IDL input.

#include "ast_expression.h"
#include "ast_constant.h"
#include "ast_typedef.h"
#include "ast_param_holder.h"
#include "ast_visitor.h"
#include "ast_generator.h"
#include "ast_enum_val.h"

#include "utl_err.h"
#include "utl_scope.h"
#include "utl_string.h"

#include "nr_extern.h"
#include "global_extern.h"

#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

AST_Expression::ExprType
AST_Expression::eval_kind_to_expr_type (AST_Expression::EvalKind eval_kind)
{
  switch (eval_kind)
    {
    case EK_bool:
      return EV_bool;
    case EK_short:
      return EV_short;
    case EK_ushort:
      return EV_ushort;
    case EK_long:
      return EV_long;
    case EK_ulong:
    case EK_positive_int:
      return EV_ulong;
    case EK_longlong:
      return EV_longlong;
    case EK_ulonglong:
      return EV_ulonglong;
    case EK_octet:
      return EV_octet;
    case EK_floating_point:
      return EV_double;
    case EK_fixed_point:
      return EV_fixed;
    case EK_int8:
      return EV_int8;
    case EK_uint8:
      return EV_uint8;
    case EK_const:
      idl_global->err ()->misc_error ("eval_kind_to_expr_type can't handle EK_const");
      return EV_none;
    }

  idl_global->err ()->misc_error ("eval_kind_to_expr_type unhandled EvalKind");
  return EV_none;
}

// Helper function to fill out the details of where this expression
// is defined.
void
AST_Expression::fill_definition_details ()
{
  this->pd_defined_in = idl_global->scopes ().depth () > 0
                          ? idl_global->scopes ().top ()
                          : nullptr ;
  this->pd_line = idl_global->lineno ();
  this->pd_file_name = idl_global->filename ();
  enum_parent (nullptr);
}

// An AST_Expression denoting a symbolic name.
AST_Expression::AST_Expression (UTL_ScopedName *nm)
  : pd_ec (EC_symbol),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nm),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  AST_Decl *d =
    idl_global->scopes ().top_non_null ()->lookup_by_name (nm,
                                                           true);

  if (d->node_type () == AST_Decl::NT_param_holder)
    {
      this->param_holder_ =
        dynamic_cast<AST_Param_Holder*> (d);
    }
}

// An AST_Expression denoting a type coercion from another AST_Expression.
AST_Expression::AST_Expression (AST_Expression *v,
                                ExprType t)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  AST_Param_Holder *ph = v->param_holder_;
  this->fill_definition_details ();
  enum_parent (v->enum_parent ());

  // If we are here because one string constant has
  // another one as its rhs, we must copy the UTL_String
  // so both can be destroyed at cleanup.
  if (EV_string == t)
    {
      ACE_NEW (this->pd_ev,
              AST_ExprValue);

      ACE_NEW (this->pd_ev->u.strval,
               UTL_String (v->pd_ev->u.strval, true));

      this->pd_ev->et = EV_string;
    }
  else if (EV_wstring == t)
    {
      ACE_NEW (this->pd_ev,
              AST_ExprValue);

      this->pd_ev->u.wstrval = ACE::strnew (v->pd_ev->u.wstrval);
      this->pd_ev->et = EV_string;
    }
  else
    {
      if (ph == nullptr)
        {
          this->pd_ev = v->coerce (t);

          if (this->pd_ev == nullptr)
            {
              idl_global->err ()->coercion_error (v, t);
            }
        }
      else
        {
          // The param info member isn't managed by this type,
          // so we can just pass the pointer to the new expr.
          this->param_holder_ =
            idl_global->gen ()->create_param_holder (
              ph->name (),
              const_cast<FE_Utils::T_Param_Info *> (ph->info ()));
        }

      if (nullptr != v->pd_n)
        {
          this->pd_n =
            dynamic_cast<UTL_ScopedName *> (v->pd_n->copy ());
        }
    }
}

// An AST_Expression denoting a binary expression combination from
// two other AST_Expressions.
AST_Expression::AST_Expression (ExprComb c,
                                AST_Expression *ev1,
                                AST_Expression *ev2)
  : pd_ec (c),
    pd_ev (nullptr),
    pd_v1 (ev1),
    pd_v2 (ev2),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();
}

// An AST_Expression denoting a short integer.
AST_Expression::AST_Expression (ACE_CDR::Short sv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_short;
  this->pd_ev->u.sval = sv;
}

// An AST_Expression denoting an unsigned short integer.
AST_Expression::AST_Expression (ACE_CDR::UShort usv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_ushort;
  this->pd_ev->u.usval = usv;
}

// An AST_Expression denoting a long integer.
AST_Expression::AST_Expression (ACE_CDR::Long lv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_long;
  this->pd_ev->u.lval = lv;
}

// An AST_Expression denoting a long integer.
AST_Expression::AST_Expression (ACE_CDR::LongLong llv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_longlong;
  this->pd_ev->u.llval = llv;
}

// An AST_Expression denoting a boolean.
AST_Expression::AST_Expression (ACE_CDR::Boolean b)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_bool;
  this->pd_ev->u.bval = b;
}

// An AST_Expression denoting an unsigned long integer.
AST_Expression::AST_Expression (ACE_CDR::ULong ulv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_ulong;
  this->pd_ev->u.ulval = ulv;
}

// An AST_Expression denoting an unsigned long long integer.
AST_Expression::AST_Expression (ACE_CDR::ULongLong ulv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_ulonglong;
  this->pd_ev->u.ullval = ulv;
}

// An AST_Expression denoting an unsigned long integer.
AST_Expression::AST_Expression (ACE_CDR::ULong ulv,
                                ExprType t)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = t;
  this->pd_ev->u.ullval = ulv;
}

// An AST_Expression denoting a 32-bit floating point number.
AST_Expression::AST_Expression (ACE_CDR::Float fv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_float;
  this->pd_ev->u.fval = fv;
}

// An AST_Expression denoting a 64-bit floating point number.
AST_Expression::AST_Expression (ACE_CDR::Double dv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_double;
  this->pd_ev->u.dval = dv;
}

// An AST_Expression denoting a character.
AST_Expression::AST_Expression (ACE_CDR::Char cv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_char;
  this->pd_ev->u.cval = cv;
}

// An AST_Expression denoting a wide character.
AST_Expression::AST_Expression (ACE_OutputCDR::from_wchar wcv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_wchar;
  this->pd_ev->u.wcval = wcv.val_;
}

// An AST_Expression denoting an octet (unsigned char).
AST_Expression::AST_Expression (ACE_CDR::Octet ov)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_octet;
  this->pd_ev->u.oval = ov;
}

// An AST_Expression denoting a string (char * encapsulated as a String).
AST_Expression::AST_Expression (UTL_String *sv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  UTL_String *new_str = nullptr;
  ACE_NEW (new_str,
           UTL_String (sv, true));

  this->pd_ev->u.strval = new_str;
  this->pd_ev->et = EV_string;
}

// An AST_Expression denoting a wide string.
AST_Expression::AST_Expression (char *sv)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_wstring;
  this->pd_ev->u.wstrval = ACE::strnew (sv);
}

AST_Expression::AST_Expression (const ACE_CDR::Fixed &f)
  : pd_ec (EC_none),
    pd_ev (nullptr),
    pd_v1 (nullptr),
    pd_v2 (nullptr),
    pd_n (nullptr),
    tdef (nullptr),
    param_holder_ (nullptr)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_fixed;
  this->pd_ev->u.fixedval = f;
}

AST_Expression::~AST_Expression ()
{
  destroy ();
}

AST_Expression::AST_ExprValue::AST_ExprValue ()
{
  this->u.ulval = 0UL;
  this->et = AST_Expression::EV_none;
}

// Static operations.

// Perform the coercion from the given AST_ExprValue to the requested
// ExprType. Return an AST_ExprValue if successful, 0 if failed.
static AST_Expression::AST_ExprValue *
coerce_value (AST_Expression::AST_ExprValue *ev,
              AST_Expression::ExprType t)
{
  if (ev == nullptr)
    {
      return nullptr;
    }

  if (t == ev->et)
    {
      return ev;
    }

  // Avoid Coverity issue with "Assignment of overlapping memory" by using a
  // temporary value and setting ev->u at the end if there wasn't an error.
  AST_Expression::AST_ExprValue::Value tmp;
  switch (t)
    {
    case AST_Expression::EV_short:
      switch (ev->et)
        {
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (ACE_CDR::UShort) ACE_INT16_MAX)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval > (ACE_CDR::Long) ACE_INT16_MAX
              || ev->u.lval < (ACE_CDR::Long) ACE_INT16_MIN)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (ACE_CDR::ULong) ACE_INT16_MAX)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_INT16_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_INT16_MIN)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_INT16_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.sval = (ACE_CDR::Short) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval > (ACE_CDR::Float) ACE_INT16_MAX
              || ev->u.fval < (ACE_CDR::Float) ACE_INT16_MIN)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > (ACE_CDR::Double) ACE_INT16_MAX
              || ev->u.dval < (ACE_CDR::Double) ACE_INT16_MIN)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          tmp.sval = (ACE_CDR::Short) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_INT16_MAX)
            {
              return nullptr;
            }

          tmp.sval = (ACE_CDR::Short) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.sval = (ACE_CDR::Short) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          tmp.sval = (ACE_CDR::Short) ev->u.int8val;
          break;
        case AST_Expression::EV_uint8:
          tmp.sval = (ACE_CDR::Short) ev->u.uint8val;
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_ushort:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.sval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval > (ACE_CDR::Long) ACE_UINT16_MAX
              || ev->u.lval < 0)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (ACE_CDR::ULong) ACE_UINT16_MAX)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_UINT16_MAX
              || ev->u.llval < 0)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_UINT16_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.usval = (ACE_CDR::UShort) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (ACE_CDR::Float) ACE_UINT16_MAX)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (ACE_CDR::Double) ACE_UINT16_MAX)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
              return nullptr;
            }

          tmp.usval = (ACE_CDR::UShort) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.usval = (ACE_CDR::UShort) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.usval = (ACE_CDR::UShort) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          if (ev->u.int8val < 0) return nullptr;
          tmp.usval = static_cast<ACE_CDR::UShort> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.usval = static_cast<ACE_CDR::UShort> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_long:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          tmp.lval = (ACE_CDR::Long) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.lval = (ACE_CDR::Long) ev->u.usval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (ACE_CDR::ULong) ACE_INT32_MAX)
            {
              return nullptr;
            }

          tmp.lval = (ACE_CDR::Long) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_INT32_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_INT32_MIN)
            {
              return nullptr;
            }
          tmp.lval = (ACE_CDR::Long) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_INT32_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.lval = (ACE_CDR::Long) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.lval = (ACE_CDR::Long) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval > (ACE_CDR::Float) ACE_INT32_MAX
              || ev->u.fval < (ACE_CDR::Float) ACE_INT32_MIN)
            {
              return nullptr;
            }

          tmp.lval = (ACE_CDR::Long) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > (ACE_CDR::Double) ACE_INT32_MAX
              || ev->u.dval < (ACE_CDR::Double) ACE_INT32_MIN)
            {
              return nullptr;
            }

          tmp.lval = (ACE_CDR::Long) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          tmp.lval = (ACE_CDR::Long) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.lval = (ACE_CDR::Long) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.lval = (ACE_CDR::Long) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          tmp.lval = static_cast<ACE_CDR::Long> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.lval = static_cast<ACE_CDR::Long> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_ulong:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.ulval = (ACE_CDR::ULong) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.lval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_UINT32_MAX
              || ev->u.llval < 0)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_UINT32_MAX) != ev->u.ullval)
            {
              return nullptr;
            }
          tmp.ulval = (ACE_CDR::ULong) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.ulval = (ACE_CDR::ULong) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (ACE_CDR::Float) ACE_UINT32_MAX)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (ACE_CDR::Double) ACE_UINT32_MAX)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
              return nullptr;
            }

          tmp.ulval = (ACE_CDR::ULong) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.ulval = (ACE_CDR::ULong) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.ulval = (ACE_CDR::ULong) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          if (ev->u.int8val < 0) return nullptr;
          tmp.ulval = static_cast<ACE_CDR::ULong> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.ulval = static_cast<ACE_CDR::ULong> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_longlong:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          tmp.llval = (ACE_CDR::LongLong) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.llval = (ACE_CDR::LongLong) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          tmp.llval = (ACE_CDR::LongLong) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          tmp.llval = (ACE_CDR::LongLong) ev->u.ulval;
          break;
        case AST_Expression::EV_ulonglong:
          if (ev->u.ullval > ACE_INT64_MAX)
            {
              return nullptr;
            }

          tmp.llval = static_cast<ACE_CDR::LongLong> (ev->u.ullval);
          break;
        case AST_Expression::EV_bool:
          tmp.llval = (ACE_CDR::LongLong) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval > (ACE_CDR::Float) ACE_INT64_MAX
              || ev->u.fval < (ACE_CDR::Float) ACE_INT64_MIN)
            {
              return nullptr;
            }

          tmp.llval = (ACE_CDR::LongLong) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > (ACE_CDR::Double) ACE_INT64_MAX
              || ev->u.dval < (ACE_CDR::Double) ACE_INT64_MIN)
            {
              return nullptr;
            }

          tmp.llval = (ACE_CDR::LongLong) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          tmp.llval = (ACE_CDR::LongLong) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.llval = (ACE_CDR::LongLong) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.llval = (ACE_CDR::LongLong) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          tmp.llval = static_cast<ACE_CDR::LongLong> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.llval = static_cast<ACE_CDR::LongLong> (ev->u.int8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_ulonglong:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
              return nullptr;
            }

          tmp.ullval = ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.ullval = ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0)
            {
              return nullptr;
            }

          tmp.ullval = ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          tmp.ullval = ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval < 0)
            {
              return nullptr;
            }

          tmp.ullval = static_cast<ACE_CDR::LongLong> (ev->u.llval);
          break;
        case AST_Expression::EV_bool:
          tmp.ullval = ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (ACE_CDR::Float) ACE_UINT64_MAX)
            {
              return nullptr;
            }

          tmp.ullval = static_cast<ACE_CDR::ULongLong> (ev->u.fval);
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (ACE_CDR::Double) ACE_UINT64_MAX)
            {
              return nullptr;
            }

          tmp.ullval = static_cast<ACE_CDR::ULongLong> (ev->u.dval);
          break;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
              return nullptr;
            }

          tmp.ullval = ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.ullval = ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.ullval = ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          if (ev->u.int8val < 0) return nullptr;
          tmp.ullval = static_cast<ACE_CDR::ULongLong> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.ullval = static_cast<ACE_CDR::ULongLong> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_bool:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          tmp.bval = (ev->u.sval == 0) ? false : true;
          break;
        case AST_Expression::EV_ushort:
          tmp.bval = (ev->u.usval == 0) ? false : true;
          break;
        case AST_Expression::EV_long:
          tmp.bval = (ev->u.lval == 0) ? false : true;
          break;
        case AST_Expression::EV_ulong:
          tmp.bval = (ev->u.ulval == 0) ? false : true;
          break;
        case AST_Expression::EV_longlong:
          tmp.bval = (ev->u.llval == 0) ? false : true;
          break;
        case AST_Expression::EV_ulonglong:
          tmp.bval = (ev->u.ullval == 0) ? false : true;
          break;
        case AST_Expression::EV_float:
          tmp.bval = ACE::is_equal (ev->u.fval, 0.0f) ? false : true;
          break;
        case AST_Expression::EV_double:
          tmp.bval = ACE::is_equal (ev->u.dval, 0.0) ? false : true;
          break;
        case AST_Expression::EV_char:
          tmp.bval = (ev->u.cval == 0) ? false : true;
          break;
        case AST_Expression::EV_wchar:
          tmp.bval = (ev->u.wcval == 0) ? false : true;
          break;
        case AST_Expression::EV_octet:
          tmp.bval = (ev->u.oval == 0) ? false : true;
          break;
        case AST_Expression::EV_int8:
          tmp.bval = ev->u.int8val ? true : false;
          break;
        case AST_Expression::EV_uint8:
          tmp.bval = ev->u.uint8val ? true : false;
          break;
        default:
         return nullptr;
        }
      break;

    case AST_Expression::EV_float:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          tmp.fval = (ACE_CDR::Float) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.fval = (ACE_CDR::Float) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          tmp.fval = (ACE_CDR::Float) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          tmp.fval = (ACE_CDR::Float) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > ACE_FLT_MAX
              || ev->u.llval < ACE_FLT_LOWEST)
            {
              return nullptr;
            }
          tmp.fval = (ACE_CDR::Float) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          tmp.fval = (ACE_CDR::Float) ((ACE_CDR::LongLong) ev->u.ullval);
          break;
        case AST_Expression::EV_bool:
          tmp.fval = ev->u.bval ? 1.0f : 0.0f;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > ACE_FLT_MAX
              || ev->u.dval < ACE_FLT_LOWEST)
            {
              return nullptr;
            }

          tmp.fval = (ACE_CDR::Float) ev->u.dval;
          break;
        case AST_Expression::EV_char:
          tmp.fval = (ACE_CDR::Float) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.fval = (ACE_CDR::Float) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.fval = (ACE_CDR::Float) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          tmp.fval = static_cast<ACE_CDR::Float> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.fval = static_cast<ACE_CDR::Float> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_double:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          tmp.dval = (ACE_CDR::Double) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.dval = (ACE_CDR::Double) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          tmp.dval = (ACE_CDR::Double) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          tmp.dval = (ACE_CDR::Double) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          tmp.dval = (ACE_CDR::Double) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          // Some compilers don't implement unsigned 64-bit to double
          // conversions, so we are stuck with the signed 64-bit max value.
          if (ev->u.ullval > ACE_INT64_MAX)
            {
              return nullptr;
            }

          tmp.dval = (ACE_CDR::Double) ((ACE_CDR::LongLong) ev->u.ullval);
          break;
        case AST_Expression::EV_bool:
          tmp.dval = ev->u.bval ? 1.0 : 0.0;
          break;
        case AST_Expression::EV_float:
          tmp.dval = (ACE_CDR::Double) ev->u.fval;
          break;
        case AST_Expression::EV_char:
          tmp.dval = (ACE_CDR::Double) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          tmp.dval = (ACE_CDR::Double) ev->u.wcval;
          break;
        case AST_Expression::EV_octet:
          tmp.dval = (ACE_CDR::Double) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
          tmp.dval = static_cast<ACE_CDR::Double> (ev->u.int8val);
          break;
        case AST_Expression::EV_uint8:
          tmp.dval = static_cast<ACE_CDR::Double> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_int8:
    case AST_Expression::EV_char:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval > (ACE_CDR::Short) ACE_CHAR_MAX
              || ev->u.sval < (ACE_CDR::Short) ACE_CHAR_MIN)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (ACE_CDR::UShort) ACE_CHAR_MAX)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval > (ACE_CDR::Long) ACE_CHAR_MAX
              || ev->u.lval < (ACE_CDR::Long) ACE_CHAR_MIN)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (ACE_CDR::ULong) ACE_CHAR_MAX)
            {
                    return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_CHAR_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_CHAR_MIN)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_CHAR_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.cval = (ACE_CDR::Char) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval > (ACE_CDR::Float) ACE_CHAR_MAX
              || ev->u.fval < (ACE_CDR::Float) ACE_CHAR_MIN)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > (ACE_CDR::Double) ACE_CHAR_MAX
              || ev->u.dval < (ACE_CDR::Double) ACE_CHAR_MIN)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.dval;
          break;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_CHAR_MAX)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.wcval;
          break;
        case AST_Expression::EV_uint8:
        case AST_Expression::EV_octet:
          if (ev->u.oval > (unsigned char) ACE_CHAR_MAX)
            {
              return nullptr;
            }

          tmp.cval = (ACE_CDR::Char) ev->u.oval;
          break;
        case AST_Expression::EV_int8:
        case AST_Expression::EV_char:
          tmp = ev->u;
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_wchar:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          tmp.wcval = (ACE_CDR::WChar) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0
              || ev->u.lval > ACE_WCHAR_MAX)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > ACE_WCHAR_MAX)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_WCHAR_MAX
              || ev->u.llval < 0)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_WCHAR_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.wcval = (ACE_CDR::WChar) ev->u.bval;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval > (ACE_CDR::Float) ACE_WCHAR_MAX
              || ev->u.fval < 0)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval > (ACE_CDR::Double) ACE_WCHAR_MAX
              || ev->u.dval < 0)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.dval;
          break;
        case AST_Expression::EV_char:
        case AST_Expression::EV_int8:
          if ((signed char) ev->u.cval < 0)
            {
              return nullptr;
            }

          tmp.wcval = (ACE_CDR::WChar) ev->u.cval;
          break;
        case AST_Expression::EV_octet:
          tmp.wcval = (ACE_CDR::WChar) ev->u.oval;
          break;
        case AST_Expression::EV_uint8:
          tmp.wcval = static_cast<ACE_CDR::WChar> (ev->u.uint8val);
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_uint8:
    case AST_Expression::EV_octet:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0
              || ev->u.sval > (ACE_CDR::Short) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.sval;
          break;
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (ACE_CDR::UShort) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.usval;
          break;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0
              || ev->u.lval > (ACE_CDR::Long) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.lval;
          break;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (ACE_CDR::ULong) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.ulval;
          break;
        case AST_Expression::EV_longlong:
          if (ev->u.llval < 0
              || ev->u.llval > (ACE_CDR::LongLong) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.llval;
          break;
        case AST_Expression::EV_ulonglong:
          if ((ev->u.ullval & ACE_OCTET_MAX) != ev->u.ullval)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.ullval;
          break;
        case AST_Expression::EV_bool:
          tmp.oval = (ACE_CDR::Octet) ev->u.bval ? 1 : 0;
          break;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (ACE_CDR::Float) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.fval;
          break;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (ACE_CDR::Double) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.dval;
          break;
        case AST_Expression::EV_int8:
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.cval;
          break;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_OCTET_MAX)
            {
              return nullptr;
            }

          tmp.oval = (ACE_CDR::Octet) ev->u.wcval;
          break;
        case AST_Expression::EV_uint8:
        case AST_Expression::EV_octet:
          tmp = ev->u;
          break;
        default:
          return nullptr;
        }
      break;

    case AST_Expression::EV_enum:
      switch (ev->et)
        {
        case AST_Expression::EV_ulong:
          return ev;
        default:
          return nullptr;
        }
    default:
      return nullptr;
    }

  ev->et = t;
  ev->u = tmp;
  return ev;
}

// Integer literals may not be assigned to floating point constants,
// and vice versa.
static bool
incompatible_types (AST_Expression::ExprType t1,
                    AST_Expression::ExprType t2)
{
  switch (t1)
  {
    case AST_Expression::EV_short:
    case AST_Expression::EV_ushort:
    case AST_Expression::EV_long:
    case AST_Expression::EV_ulong:
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
    case AST_Expression::EV_octet:
    case AST_Expression::EV_bool:
    case AST_Expression::EV_int8:
    case AST_Expression::EV_uint8:
      switch (t2)
      {
        case AST_Expression::EV_short:
        case AST_Expression::EV_ushort:
        case AST_Expression::EV_long:
        case AST_Expression::EV_ulong:
        case AST_Expression::EV_longlong:
        case AST_Expression::EV_ulonglong:
        case AST_Expression::EV_octet:
        case AST_Expression::EV_bool:
        case AST_Expression::EV_int8:
        case AST_Expression::EV_uint8:
          return false;
        default:
          return true;
      }
    case AST_Expression::EV_float:
    case AST_Expression::EV_double:
    case AST_Expression::EV_longdouble:
      switch (t2)
      {
        case AST_Expression::EV_float:
        case AST_Expression::EV_double:
        case AST_Expression::EV_longdouble:
          return false;
        default:
          return true;
      }
    case AST_Expression::EV_fixed:
      return t2 != AST_Expression::EV_fixed;
    default:
      return false;
  }
}

// Evaluate the expression wrt the evaluation kind requested. Supported
// evaluation kinds are
// - EK_const:          The expression must evaluate to a constant
// - EK_positive_int:   The expression must further evaluate to a
// positive integer

// @@(JP) This just maps one enum to another. It's a temporary fix,
// but AST_Expression::EvalKind should go eventually.
static AST_Expression::AST_ExprValue *
eval_kind (AST_Expression::AST_ExprValue *ev, AST_Expression::EvalKind ek)
{
  // Make a copy to simplify the memory management logic.
  AST_Expression::AST_ExprValue *newval = nullptr;
  ACE_NEW_RETURN (newval,
                  AST_Expression::AST_ExprValue,
                  nullptr);

  if (ev != nullptr)
    {
      *newval = *ev;
    }

  AST_Expression::AST_ExprValue *retval = nullptr;

  switch (ek)
  {
    case AST_Expression::EK_const:
      retval = newval;
      break;
    case AST_Expression::EK_positive_int:
      retval = coerce_value (newval, AST_Expression::EV_ulong);
      break;
    case AST_Expression::EK_short:
      retval = coerce_value (newval, AST_Expression::EV_short);
      break;
    case AST_Expression::EK_ushort:
      retval = coerce_value (newval, AST_Expression::EV_ushort);
      break;
    case AST_Expression::EK_long:
      retval = coerce_value (newval, AST_Expression::EV_long);
      break;
    case AST_Expression::EK_ulong:
      retval = coerce_value (newval, AST_Expression::EV_ulong);
      break;
    case AST_Expression::EK_longlong:
      retval = coerce_value (newval, AST_Expression::EV_longlong);
      break;
    case AST_Expression::EK_ulonglong:
      retval = coerce_value (newval, AST_Expression::EV_ulonglong);
      break;
    case AST_Expression::EK_octet:
      retval = coerce_value (newval, AST_Expression::EV_octet);
      break;
    case AST_Expression::EK_bool:
      retval = coerce_value (newval, AST_Expression::EV_bool);
      break;
    case AST_Expression::EK_fixed_point:
      retval = coerce_value (newval, AST_Expression::EV_fixed);
      break;
    case AST_Expression::EK_int8:
      retval = coerce_value (newval, AST_Expression::EV_int8);
      break;
    case AST_Expression::EK_uint8:
      retval = coerce_value (newval, AST_Expression::EV_uint8);
      break;
    case AST_Expression::EK_floating_point:
      retval = coerce_value (newval, AST_Expression::EV_double);
      break;
  }

  // Sometimes the call above to coerce_value() will return an
  // evaluated newval, other times 0. But a heap-allocated
  // ExprValue is not always passed to coerce_value(), so we
  // have to manage it here, where we know it is always a 'new'.
  if (retval != newval)
    {
      delete newval;
      newval = nullptr;
    }

  return retval;
}

// Private operations.

// @@@ (JP) CORBA 2.6 and earlier say that in a constant expression,
// each subexpression must fall within the range of the assigned type.
// However, this may be hard for the compiler in some cases (must
// evaluate all grouping possibilities). So there is an outstanding
// issue, #1139, and the best guess is that it will ultimately be
// decided that only the final value must fall within the range of
// the assigned type. So there are no checks here, only in coerce().

// Apply binary operators to an AST_Expression after evaluating
// its sub-expressions.
// Operations supported: '+', '-', '*', '/'
template <typename Type>
bool
do_eval_bin_op (AST_Expression::ExprComb op, Type a, Type b, Type &result)
{
  switch (op)
    {
    case AST_Expression::EC_add:
      result = a + b;
      break;
    case AST_Expression::EC_minus:
      result = a - b;
      break;
    case AST_Expression::EC_mul:
      result = a * b;
      break;
    case AST_Expression::EC_div:
      if (!b) return false;
      result = a / b;
      break;
    default:
      return false;
    }

  return true;
}

template <typename Type>
bool
do_eval_bin_op_float (AST_Expression::ExprComb op, Type a, Type b, Type &result)
{
  if (op == AST_Expression::EC_div)
    {
      result = a / b;
      return true;
    }
  return do_eval_bin_op (op, a, b, result);
}

AST_Expression::AST_ExprValue *
AST_Expression::eval_bin_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = nullptr;

  if (this->pd_v1 == nullptr || this->pd_v2 == nullptr)
    {
      return nullptr;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == nullptr || this->pd_v2->ev () == nullptr)
    {
      return nullptr;
    }

  ExprType const expr_type = ek == EK_const ?
    pd_v1->ev ()->et : eval_kind_to_expr_type (ek);
  if (expr_type == EV_none) return nullptr;

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  nullptr);

  pd_v1->set_ev (pd_v1->coerce (expr_type));
  pd_v2->set_ev (pd_v2->coerce (expr_type));
  retval->et = expr_type;

  bool success = false;
  switch (expr_type)
    {
    case EV_int8:
      success = do_eval_bin_op<ACE_CDR::Int8> (pd_ec,
        pd_v1->ev ()->u.int8val, pd_v2->ev ()->u.int8val, retval->u.int8val);
      break;

    case EV_uint8:
      success = do_eval_bin_op<ACE_CDR::UInt8> (pd_ec,
        pd_v1->ev ()->u.uint8val, pd_v2->ev ()->u.uint8val, retval->u.uint8val);
      break;

    case EV_short:
      success = do_eval_bin_op<ACE_CDR::Short> (pd_ec,
        pd_v1->ev ()->u.sval, pd_v2->ev ()->u.sval, retval->u.sval);
      break;

    case EV_ushort:
      success = do_eval_bin_op<ACE_CDR::UShort> (pd_ec,
        pd_v1->ev ()->u.usval, pd_v2->ev ()->u.usval, retval->u.usval);
      break;

    case EV_long:
      success = do_eval_bin_op<ACE_CDR::Long> (pd_ec,
        pd_v1->ev ()->u.lval, pd_v2->ev ()->u.lval, retval->u.lval);
      break;

    case EV_ulong:
      success = do_eval_bin_op<ACE_CDR::ULong> (pd_ec,
        pd_v1->ev ()->u.ulval, pd_v2->ev ()->u.ulval, retval->u.ulval);
      break;

    case EV_longlong:
      success = do_eval_bin_op<ACE_CDR::LongLong> (pd_ec,
        pd_v1->ev ()->u.llval, pd_v2->ev ()->u.llval, retval->u.llval);
      break;

    case EV_ulonglong:
      success = do_eval_bin_op<ACE_CDR::ULongLong> (pd_ec,
        pd_v1->ev ()->u.ullval, pd_v2->ev ()->u.ullval, retval->u.ullval);
      break;

    case EV_octet:
      success = do_eval_bin_op<ACE_CDR::Octet> (pd_ec,
        pd_v1->ev ()->u.oval, pd_v2->ev ()->u.oval, retval->u.oval);
      break;

    case EV_double:
      success = do_eval_bin_op_float<ACE_CDR::Double> (pd_ec,
        pd_v1->ev ()->u.dval, pd_v2->ev ()->u.dval, retval->u.dval);
      break;

    case EV_fixed:
      success = do_eval_bin_op<ACE_CDR::Fixed> (pd_ec,
        pd_v1->ev ()->u.fixedval, pd_v2->ev ()->u.fixedval, retval->u.fixedval);
      break;

    default:
      break;
    }

  if (!success)
    {
      delete retval;
      retval = nullptr;
    }

  return retval;
}

template <typename Type>
bool
do_eval_mod_op (Type a, Type b, Type &result)
{
  if (b == 0) {
    return false;
  }
  result = a % b;
  return true;
}

// Apply binary operators to an AST_Expression after evaluating
// its sub-expressions.
// Operations supported: '%'
AST_Expression::AST_ExprValue *
AST_Expression::eval_mod_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = nullptr;

  if (this->pd_v1 == nullptr || this->pd_v2 == nullptr)
    {
      return nullptr;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == nullptr || this->pd_v2->ev () == nullptr)
    {
      return nullptr;
    }

  ExprType const expr_type = ek == EK_const ?
    pd_v1->ev ()->et : eval_kind_to_expr_type (ek);
  if (expr_type == EV_none) return nullptr;

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  nullptr);

  pd_v1->set_ev (pd_v1->coerce (expr_type));
  pd_v2->set_ev (pd_v2->coerce (expr_type));
  retval->et = expr_type;

  bool success = false;
  switch (expr_type)
    {
    case EV_int8:
      success = do_eval_mod_op<ACE_CDR::Int8> (
        pd_v1->ev ()->u.int8val, pd_v2->ev ()->u.int8val, retval->u.int8val);
      break;

    case EV_uint8:
      success = do_eval_mod_op<ACE_CDR::UInt8> (
        pd_v1->ev ()->u.uint8val, pd_v2->ev ()->u.uint8val, retval->u.uint8val);
      break;

    case EV_short:
      success = do_eval_mod_op<ACE_CDR::Short> (
        pd_v1->ev ()->u.sval, pd_v2->ev ()->u.sval, retval->u.sval);
      break;

    case EV_ushort:
      success = do_eval_mod_op<ACE_CDR::UShort> (
        pd_v1->ev ()->u.usval, pd_v2->ev ()->u.usval, retval->u.usval);
      break;

    case EV_long:
      success = do_eval_mod_op<ACE_CDR::Long> (
        pd_v1->ev ()->u.lval, pd_v2->ev ()->u.lval, retval->u.lval);
      break;

    case EV_ulong:
      success = do_eval_mod_op<ACE_CDR::ULong> (
        pd_v1->ev ()->u.ulval, pd_v2->ev ()->u.ulval, retval->u.ulval);
      break;

    case EV_longlong:
      success = do_eval_mod_op<ACE_CDR::LongLong> (
        pd_v1->ev ()->u.llval, pd_v2->ev ()->u.llval, retval->u.llval);
      break;

    case EV_ulonglong:
      success = do_eval_mod_op<ACE_CDR::ULongLong> (
        pd_v1->ev ()->u.ullval, pd_v2->ev ()->u.ullval, retval->u.ullval);
      break;

    case EV_octet:
      success = do_eval_mod_op<ACE_CDR::Octet> (
        pd_v1->ev ()->u.oval, pd_v2->ev ()->u.oval, retval->u.oval);
      break;

    default:
      break;
    }

  if (!success)
    {
      delete retval;
      retval = nullptr;
    }

  return retval;
}

// Apply bitwise operations to an AST_Expression after evaluating
// its sub-expressions.
// Operations supported: '%', '|', '&', '^', '<<', '>>'

template <typename Type>
bool
do_eval_bit_op_no_shift (AST_Expression::ExprComb op, Type a, Type b, Type &result)
{
  switch (op)
    {
    case AST_Expression::EC_or:
      result = a | b;
      break;
    case AST_Expression::EC_xor:
      result = a ^ b;
      break;
    case AST_Expression::EC_and:
      result = a & b;
      break;
    default:
      return false;
    }

  return true;
}

template <typename Type>
bool
do_eval_bit_op (AST_Expression::ExprComb op, Type a, Type b, Type &result)
{
  switch (op)
    {
    case AST_Expression::EC_left:
      result = a << b;
      break;
    case AST_Expression::EC_right:
      result = a >> b;
      break;
    default:
      return do_eval_bit_op_no_shift (op, a, b, result);
    }

  return true;
}

AST_Expression::AST_ExprValue *
AST_Expression::eval_bit_op (AST_Expression::EvalKind ek)
{
  AST_Expression::AST_ExprValue *retval = nullptr;

  if (this->pd_v1 == nullptr || this->pd_v2 == nullptr)
    {
      return nullptr;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == nullptr || this->pd_v2->ev () == nullptr)
    {
      return nullptr;
    }

  ExprType const expr_type = eval_kind_to_expr_type (ek);
  if (expr_type == EV_none) return nullptr;

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  nullptr);

  pd_v1->set_ev (pd_v1->coerce (expr_type));
  pd_v2->set_ev (pd_v2->coerce (expr_type));
  retval->et = expr_type;

  bool success = false;
  switch (expr_type)
    {
    case EV_int8:
      success = do_eval_bit_op<ACE_CDR::Int8> (pd_ec,
        pd_v1->ev ()->u.int8val, pd_v2->ev ()->u.int8val, retval->u.int8val);
      break;

    case EV_uint8:
      success = do_eval_bit_op<ACE_CDR::UInt8> (pd_ec,
        pd_v1->ev ()->u.uint8val, pd_v2->ev ()->u.uint8val, retval->u.uint8val);
      break;

    case EV_short:
      success = do_eval_bit_op<ACE_CDR::Short> (pd_ec,
        pd_v1->ev ()->u.sval, pd_v2->ev ()->u.sval, retval->u.sval);
      break;

    case EV_ushort:
      success = do_eval_bit_op<ACE_CDR::UShort> (pd_ec,
        pd_v1->ev ()->u.usval, pd_v2->ev ()->u.usval, retval->u.usval);
      break;

    case EV_long:
      success = do_eval_bit_op<ACE_CDR::Long> (pd_ec,
        pd_v1->ev ()->u.lval, pd_v2->ev ()->u.lval, retval->u.lval);
      break;

    case EV_ulong:
      success = do_eval_bit_op<ACE_CDR::ULong> (pd_ec,
        pd_v1->ev ()->u.ulval, pd_v2->ev ()->u.ulval, retval->u.ulval);
      break;

    case EV_longlong:
      success = do_eval_bit_op<ACE_CDR::LongLong> (pd_ec,
        pd_v1->ev ()->u.llval, pd_v2->ev ()->u.llval, retval->u.llval);
      break;

    case EV_ulonglong:
      success = do_eval_bit_op<ACE_CDR::ULongLong> (pd_ec,
        pd_v1->ev ()->u.ullval, pd_v2->ev ()->u.ullval, retval->u.ullval);
      break;

    case EV_octet:
      success = do_eval_bit_op<ACE_CDR::Octet> (pd_ec,
        pd_v1->ev ()->u.oval, pd_v2->ev ()->u.oval, retval->u.oval);
      break;

    case EV_bool:
      success = do_eval_bit_op_no_shift<ACE_CDR::Boolean> (pd_ec,
        pd_v1->ev ()->u.bval, pd_v2->ev ()->u.bval, retval->u.bval);
      break;

    default:
      break;
    }

  if (!success)
    {
      delete retval;
      retval = nullptr;
    }

  return retval;
}

// Apply unary operators to an AST_Expression after evaluating its
// sub-expression.
// Operations supported: '-', '+', '~'
AST_Expression::AST_ExprValue *
AST_Expression::eval_un_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = nullptr;

  if (this->pd_ev != nullptr)
    {
      return this->pd_ev;
    }

  if (this->pd_v1 == nullptr)
    {
      return nullptr;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));

  if (this->pd_v1->ev () == nullptr)
    {
      return nullptr;
    }

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  nullptr);

  if (this->pd_v1->ev ()->et == EV_fixed)
    {
      retval->et = EV_fixed;
      switch (this->pd_ec)
        {
        case EC_u_plus:
          retval->u.fixedval = this->pd_v1->ev ()->u.fixedval;
          break;
        case EC_u_minus:
          retval->u.fixedval = -this->pd_v1->ev ()->u.fixedval;
          break;
        default:
          delete retval;
          return nullptr;
        }
      return retval;
    }

  retval->et = EV_double;

  switch (this->pd_ec)
    {
    case EC_u_plus:
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_double));

      if (this->pd_v1->ev () == nullptr)
        {
          delete retval;
          retval = nullptr;
          return nullptr;
        }

      retval->u.dval = this->pd_v1->ev ()->u.dval;
      break;
    case EC_u_minus:
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_double));

      if (this->pd_v1->ev () == nullptr)
        {
          delete retval;
          retval = nullptr;
          return nullptr;
        }

      retval->u.dval = -(this->pd_v1->ev ()->u.dval);
      break;
    case EC_bit_neg:
      if (this->pd_v1->ev () == nullptr)
        {
          delete retval;
          retval = nullptr;
          return nullptr;
        }

      switch (this->pd_v1->ev ()->et)
      {
        case EV_short:
          retval->u.sval = ~this->pd_v1->ev ()->u.sval;
          break;
        case EV_ushort:
          retval->u.usval = ~this->pd_v1->ev ()->u.usval;
          break;
        case EV_long:
          retval->u.lval = ~this->pd_v1->ev ()->u.lval;
          break;
        case EV_ulong:
          retval->u.ulval = ~this->pd_v1->ev ()->u.ulval;
          break;
        case EV_longlong:
          retval->u.llval = ~this->pd_v1->ev ()->u.llval;
          break;
        case EV_ulonglong:
          retval->u.ullval = ~this->pd_v1->ev ()->u.ullval;
          break;
        case EV_octet:
          retval->u.oval = ~this->pd_v1->ev ()->u.oval;
          break;
        case EV_int8:
          retval->u.int8val = ~pd_v1->ev ()->u.int8val;
          break;
        case EV_uint8:
          retval->u.uint8val = ~pd_v1->ev ()->u.uint8val;
          break;
        default:
          delete retval;
          retval = nullptr;
          return nullptr;
      }
      retval->et = pd_v1->ev ()->et;

      break;
    default:
      delete retval;
      retval = nullptr;
      return nullptr;
  }

  return retval;
}

// Evaluate a symbolic AST_Expression by looking up the named
// symbol.
AST_Expression::AST_ExprValue *
AST_Expression::eval_symbol (AST_Expression::EvalKind ek)
{
  UTL_Scope *s = nullptr;
  AST_Decl *d = nullptr;
  AST_Constant *c = nullptr;

  // Is there a symbol stored?
  if (this->pd_n == nullptr)
    {
      idl_global->err ()->eval_error (this);
      return nullptr;
    }

  // Get current scope for lookup.
  if (idl_global->scopes ().depth () > 0)
    {
      s = idl_global->scopes ().top_non_null ();
    }

  if (s == nullptr)
    {
      idl_global->err ()->lookup_error (this->pd_n);
      return nullptr;
    }

  // If we are a template parameter placeholder, just skip the
  // rest - nothing needs to be evaluated until instantiation
  // time.
  if (this->param_holder_ != nullptr)
    {
      return nullptr;
    }

  // Do lookup.
  d = s->lookup_by_name (this->pd_n,
                         true);

  if (d == nullptr)
    {
      idl_global->err ()->lookup_error (this->pd_n);
      return nullptr;
    }

  // Is it a constant?
  if (d->node_type () != AST_Decl::NT_const
      && d->node_type () != AST_Decl::NT_enum_val)
    {
      idl_global->err ()->constant_expected (this->pd_n,
                                             d);
      return nullptr;
    }

  if (d->node_type () == AST_Decl::NT_enum_val)
    {
      AST_EnumVal *enumval = dynamic_cast<AST_EnumVal*> (d);
      enum_parent (enumval->enum_parent ());
    }

  // OK, now evaluate the constant we just got, to produce its value.
  c = dynamic_cast<AST_Constant*> (d);

  if (c == nullptr)
    {
      return nullptr;
    }

  return c->constant_value ()->eval_internal (ek);
}

bool
AST_Expression::type_mismatch (AST_Expression::ExprType t)
{
  if (this->pd_ev != nullptr)
    {
      return incompatible_types (this->pd_ev->et, t);
    }

  bool v1_mismatch = false;
  bool v2_mismatch = false;

  if (this->pd_v1 != nullptr)
    {
      v1_mismatch = this->pd_v1->type_mismatch (t);
    }

  if (this->pd_v2 != nullptr)
    {
      v2_mismatch = this->pd_v2->type_mismatch (t);
    }

  return v1_mismatch | v2_mismatch;
}

// Coerce "this" to the ExprType required. Returns a copy of the
// original ExprValue with the coercion applied, if successful, or
// 0 if failed.
AST_Expression::AST_ExprValue *
AST_Expression::check_and_coerce (AST_Expression::ExprType t,
                                  AST_Decl *d)
{
  if (d != nullptr)
    {
      AST_Decl *enum_val =
        idl_global->scopes ().top_non_null ()->lookup_by_name (this->pd_n,
                                                               true);

      if (enum_val != nullptr)
        {
          AST_Decl *enum_decl = ScopeAsDecl (enum_val->defined_in ());

          if (d->node_type () == AST_Decl::NT_typedef)
            {
              AST_Typedef *td = dynamic_cast<AST_Typedef*> (d);
              d = td->primitive_base_type ();
            }

          if (d != enum_decl)
            {
              idl_global->err ()->incompatible_type_error (this);
              return nullptr;
            }
        }
    }

  if (this->type_mismatch (t))
    {
      idl_global->err ()->incompatible_type_error (this);
      return nullptr;
    }

  if (d != nullptr && d->node_type () == AST_Decl::NT_typedef)
    {
      this->tdef = d;
    }

  return this->coerce (t);
}

// Coerce "this" to the ExprType required. Returns a copy of the
// original ExprValue with the coercion applied, if successful, or
// 0 if failed.
AST_Expression::AST_ExprValue *
AST_Expression::coerce (AST_Expression::ExprType t)
{
  AST_ExprValue *tmp = nullptr;

  // First, evaluate it, then try to coerce result type.
  // If already evaluated, return the result.
  switch (t)
  {
    case EV_int8:
      tmp = this->eval_internal (EK_int8);
      break;
    case EV_uint8:
      tmp = this->eval_internal (EK_uint8);
      break;
    case EV_short:
      tmp = this->eval_internal (EK_short);
      break;
    case EV_ushort:
      tmp = this->eval_internal (EK_ushort);
      break;
    case EV_long:
      tmp = this->eval_internal (EK_long);
      break;
    case EV_ulong:
      tmp = this->eval_internal (EK_ulong);
      break;
    case EV_longlong:
      tmp = this->eval_internal (EK_longlong);
      break;
    case EV_ulonglong:
      tmp = this->eval_internal (EK_ulonglong);
      break;
    case EV_octet:
      tmp = this->eval_internal (EK_octet);
      break;
    case EV_bool:
      tmp = this->eval_internal (EK_bool);
      break;
    case EV_fixed:
      tmp = this->eval_internal (EK_fixed_point);
      break;
    case EV_float:
    case EV_double:
    case EV_longdouble:
      tmp = eval_internal (EK_floating_point);
      break;
    case EV_char:
    case EV_wchar:
    case EV_string:
    case EV_wstring:
    case EV_enum:
    case EV_any:
    case EV_object:
    case EV_void:
      tmp = eval_internal (EK_const);
    case EV_none:
      break;
  }

  if (tmp == nullptr)
    {
      return nullptr;
    }
  else
    {
      delete this->pd_ev;
      this->pd_ev = tmp;
    }

  // Create a copy to contain coercion result.
  AST_ExprValue *copy = nullptr;
  ACE_NEW_RETURN (copy,
                  AST_ExprValue,
                  nullptr);

  copy->et = this->pd_ev->et;

  switch (this->pd_ev->et)
  {
    case EV_longdouble:
    case EV_void:
    case EV_none:
      delete copy;
      copy = nullptr;
      return nullptr;
    case EV_enum:
      copy->u.ulval = this->pd_ev->u.ulval;
      break;
    case EV_short:
      copy->u.sval = this->pd_ev->u.sval;
      break;
    case EV_ushort:
      copy->u.usval = this->pd_ev->u.usval;
      break;
    case EV_long:
      copy->u.lval = this->pd_ev->u.lval;
      break;
    case EV_ulong:
      copy->u.ulval = this->pd_ev->u.ulval;
      break;
    case EV_longlong:
      copy->u.llval = this->pd_ev->u.llval;
      break;
    case EV_ulonglong:
      copy->u.ullval = this->pd_ev->u.ullval;
      break;
    case EV_bool:
      copy->u.bval = this->pd_ev->u.bval;
      break;
    case EV_float:
      copy->u.fval = this->pd_ev->u.fval;
      break;
    case EV_double:
      copy->u.dval = this->pd_ev->u.dval;
      break;
    case EV_char:
      copy->u.cval = this->pd_ev->u.cval;
      break;
    case EV_wchar:
      copy->u.wcval = this->pd_ev->u.wcval;
      break;
    case EV_octet:
      copy->u.oval = this->pd_ev->u.oval;
      break;
    case EV_string:
      copy->u.strval = this->pd_ev->u.strval;
      break;
    case EV_wstring:
      copy->u.wstrval = this->pd_ev->u.wstrval;
      break;
    case EV_fixed:
      copy->u.fixedval = this->pd_ev->u.fixedval;
      break;
    case EV_int8:
      copy->u.int8val = this->pd_ev->u.int8val;
      break;
    case EV_uint8:
      copy->u.uint8val = this->pd_ev->u.uint8val;
      break;
    case EV_any:
    case EV_object:
      break;
  }

  if (this->pd_ev->et == t)
    {
      return copy;
    }
  else
    {
      AST_ExprValue *ev = coerce_value (copy, t);

      if (ev == nullptr)
        {
          // coerce_value() will either return 'copy' or
          // return 0 without freeing 'copy'. It's
          // deleted here instead of putting a zillion
          // delete statments inside coerce_value().
          delete copy;
          copy = nullptr;
        }

      return ev;
    }
}

// Eval used internally.
AST_Expression::AST_ExprValue *
AST_Expression::eval_internal (AST_Expression::EvalKind ek)
{
  // Already evaluated?
  if (this->pd_ev != nullptr)
    {
      return eval_kind (this->pd_ev,
                        ek);
    }

  //  OK, must evaluate operator.
  switch (this->pd_ec)
    {
    case EC_add:
    case EC_minus:
    case EC_mul:
    case EC_div:
      this->pd_ev = this->eval_bin_op (ek);
      return eval_kind (this->pd_ev,
                        ek);
    case EC_mod:
      this->pd_ev = this->eval_mod_op (ek);
      return eval_kind (this->pd_ev,
                        ek);
    case EC_or:
    case EC_xor:
    case EC_and:
    case EC_left:
    case EC_right:
      this->pd_ev = this->eval_bit_op (ek);
      return eval_kind (this->pd_ev,
                        ek);
    case EC_u_plus:
    case EC_u_minus:
    case EC_bit_neg:
      this->pd_ev = this->eval_un_op (ek);
      return eval_kind (this->pd_ev,
                        ek);
    case EC_symbol:
      this->pd_ev = this->eval_symbol (ek);
      return eval_kind (this->pd_ev,
                        ek);
    case EC_none:
      return nullptr;
    }

  return nullptr;
}

// Public operations.

// Evaluate "this", assigning the value to the pd_ev field.
void
AST_Expression::evaluate (EvalKind ek)
{
  AST_ExprValue *tmp = eval_kind (this->pd_ev, ek);
  delete this->pd_ev;
  this->pd_ev = tmp;

  // Artifact of expressions doing double duty for all template
  // args. At this point, we have knowledge that we must be an
  // enum constant, so we set the expression type here, rather
  // than at the point of creation.
  if (ek == AST_Expression::EK_const && this->pd_n != nullptr)
    {
      this->pd_ev->et = AST_Expression::EV_enum;
    }
}

// Expression equality comparison operator.
bool
AST_Expression::operator== (AST_Expression *vc)
{
  return compare (vc);
}

bool
AST_Expression::compare (AST_Expression *vc)
{
  if (this->pd_ec != vc->ec ())
    {
      return false;
    }

  this->evaluate (EK_const);
  vc->evaluate (EK_const);

  if (pd_ev == nullptr || vc->ev () == nullptr)
    {
      return false;
    }

  if (this->pd_ev->et != vc->ev ()->et)
    {
      return false;
    }

  switch (pd_ev->et)
    {
    case EV_short:
      return this->pd_ev->u.sval == vc->ev ()->u.sval;
    case EV_ushort:
      return this->pd_ev->u.usval == vc->ev ()->u.usval;
    case EV_long:
      return this->pd_ev->u.lval == vc->ev ()->u.lval;
    case EV_ulong:
      return this->pd_ev->u.ulval == vc->ev ()->u.ulval;
    case EV_float:
      return ACE::is_equal (this->pd_ev->u.fval, vc->ev ()->u.fval);
    case EV_double:
      return ACE::is_equal (this->pd_ev->u.dval, vc->ev ()->u.dval);
    case EV_char:
      return this->pd_ev->u.cval == vc->ev ()->u.cval;
    case EV_wchar:
      return this->pd_ev->u.wcval == vc->ev ()->u.wcval;
    case EV_octet:
      return this->pd_ev->u.oval == vc->ev ()->u.oval;
    case EV_bool:
      return this->pd_ev->u.lval == vc->ev ()->u.lval;
    case EV_string:
      return !ACE_OS::strcmp (pd_ev->u.strval->get_string (),
        vc->ev ()->u.strval->get_string ());
    case EV_wstring:
      return !ACE_OS::strcmp (pd_ev->u.wstrval, vc->ev ()->u.wstrval);
    case EV_longlong:
      return pd_ev->u.llval == vc->ev ()->u.llval;
    case EV_ulonglong:
      return pd_ev->u.ullval == vc->ev ()->u.ullval;
    case EV_fixed:
      return pd_ev->u.fixedval == vc->ev ()->u.fixedval;
    case EV_enum:
      return pd_ev->u.eval == vc->ev ()->u.eval;
    case EV_int8:
      return pd_ev->u.int8val == vc->ev ()->u.int8val;
    case EV_uint8:
      return pd_ev->u.uint8val == vc->ev ()->u.uint8val;
    default:
      return false;
    }
}

AST_Decl *
AST_Expression::get_tdef () const
{
  return this->tdef;
}

AST_Param_Holder *
AST_Expression::param_holder () const
{
  return this->param_holder_;
}

// Helper functions for expression dumpers.

// Dump this binary AST_Expression node to the ostream o.
static void
dump_binary_expr (ACE_OSTREAM_TYPE &o,
                  const char *s,
                  AST_Expression *n1,
                  AST_Expression *n2)
{
  if (n1 != nullptr)
    {
      n1->dump (o);
    }

  o << " " << s << " ";

  if (n2 != nullptr)
    {
      n2->dump (o);
    }
}

// Dump this unary AST_Expression node to the ostream o.
static void
dump_unary_expr (ACE_OSTREAM_TYPE &o,
                 const char *s,
                 AST_Expression *e)
{
  o << s;
  e->dump (o);
}

// Dump the supplied AST_ExprValue to the ostream o.
static void
dump_expr_val (ACE_OSTREAM_TYPE &o, AST_Expression::AST_ExprValue *ev)
{
  switch (ev->et)
    {
    case AST_Expression::EV_short:
      o << ev->u.sval;
      return;
    case AST_Expression::EV_ushort:
      o << ev->u.usval;
      return;
    case AST_Expression::EV_long:
      o << ev->u.lval;
      return;
    case AST_Expression::EV_ulong:
    case AST_Expression::EV_enum:
      o << ev->u.ulval;
      return;
    case AST_Expression::EV_float:
      o << ev->u.fval;
      return;
    case AST_Expression::EV_double:
      o << ev->u.dval;
      return;
    case AST_Expression::EV_char:
      o << ev->u.cval;
      return;
    case AST_Expression::EV_wchar:
#ifdef ACE_HAS_CPP20
      break;
#else
      o << ev->u.wcval;
      return;
#endif
    case AST_Expression::EV_octet:
      {
        std::ios saved (nullptr);
        saved.copyfmt (o);
        o << "0x" << std::hex << std::setw (2) << std::setfill ('0') << unsigned (ev->u.oval);
        o.copyfmt (saved);
      }
      return;
    case AST_Expression::EV_bool:
      o << (ev->u.bval == true ? "TRUE" : "FALSE");
      return;
    case AST_Expression::EV_string:
      if (ev->u.strval != nullptr)
        {
          ev->u.strval->dump (o);
        }
      else
        {
          o << "(null string)";
        }
      return;
    case AST_Expression::EV_longlong:
      o << ev->u.llval;
      return;
    case AST_Expression::EV_ulonglong:
      o << ev->u.ullval;
      return;
    case AST_Expression::EV_fixed:
      o << ev->u.fixedval;
      return;
    case AST_Expression::EV_int8:
      o << static_cast<short> (ev->u.int8val);
      return;
    case AST_Expression::EV_uint8:
      o << static_cast<unsigned short> (ev->u.uint8val);
      return;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_any:
    case AST_Expression::EV_object:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      break;
    }

    o << "(Can not dump type " << AST_Expression::exprtype_to_string (ev->et) << ")";
}

// Dump an AST_Expression node to the ostream o.
void
AST_Expression::dump (ACE_OSTREAM_TYPE &o)
{
  // See if it was a constant or was evaluated already.
  if (this->pd_ev != nullptr)
    {
      dump_expr_val (o, this->pd_ev);
      return;
    }

  // OK, must print out an expression.
  switch (this->pd_ec)
    {
    // Binary expressions:
    case EC_add:
      dump_binary_expr (o,
                        "+",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_minus:
      dump_binary_expr (o,
                        "-",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_mul:
      dump_binary_expr (o,
                        "*",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_div:
      dump_binary_expr (o,
                        "/",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_mod:
      dump_binary_expr (o,
                        "%",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_or:
      dump_binary_expr (o,
                        "|",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_xor:
      dump_binary_expr (o,
                        "^",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_and:
      dump_binary_expr (o,
                        "&",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_left:
      dump_binary_expr (o,
                        "<<",
                        this->pd_v1,
                        this->pd_v2);
      break;
    case EC_right:
      dump_binary_expr (o,
                        ">>",
                        this->pd_v1,
                        this->pd_v2);
      break;
    // Unary expressions.
    case EC_u_plus:
      dump_unary_expr (o,
                       "+",
                       this->pd_v1);
      break;
    case EC_u_minus:
      dump_unary_expr (o,
                       "-",
                       this->pd_v1);
      break;
    case EC_bit_neg:
      dump_unary_expr (o,
                       "~",
                       this->pd_v1);
      break;
    // Unevaluated symbol.
    case EC_symbol:
      if (this->pd_n)
        this->pd_n->dump (o);
      else
        o << ACE_TEXT ("(nil symbolic name)");
      break;
    case EC_none:
      o << ACE_TEXT ("(none)");
      break;
    default:
      o << ACE_TEXT ("unsupported dump mode for expression with ec == ")
        << (int) this->pd_ec ;
      break;
    }
}

int
AST_Expression::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_expression (this);
}

void
AST_Expression::destroy ()
{
  if (nullptr != this->pd_ev)
    {
      if (EV_string == this->pd_ev->et)
        {
          this->pd_ev->u.strval->destroy ();
          delete this->pd_ev->u.strval;
          this->pd_ev->u.strval = nullptr;
        }
      else if (EV_wstring == this->pd_ev->et)
        {
          ACE::strdelete (this->pd_ev->u.wstrval);
          this->pd_ev->u.wstrval = nullptr;
        }
    }
  delete this->pd_ev;
  this->pd_ev = nullptr;

  if (this->pd_v1 != nullptr)
    {
      this->pd_v1->destroy ();
    }
  delete this->pd_v1;
  this->pd_v1 = nullptr;

  if (this->pd_v2 != nullptr)
    {
      this->pd_v2->destroy ();
    }
  delete this->pd_v2;
  this->pd_v2 = nullptr;

  if (this->pd_n != nullptr)
    {
      this->pd_n->destroy ();
    }
  delete this->pd_n;
  this->pd_n = nullptr;

  if (this->param_holder_ != nullptr)
    {
      this->param_holder_->destroy ();
      delete this->param_holder_;
      this->param_holder_ = nullptr;
    }
}

// Data accessors.

UTL_Scope *
AST_Expression::defined_in ()
{
  return this->pd_defined_in;
}

void
AST_Expression::set_defined_in (UTL_Scope *d)
{
  this->pd_defined_in = d;
}

long
AST_Expression::line ()
{
  return this->pd_line;
}

void
AST_Expression::set_line (long l)
{
  this->pd_line = l;
}

UTL_String *
AST_Expression::file_name ()
{
  return this->pd_file_name;
}

void
AST_Expression::set_file_name (UTL_String *f)
{
  this->pd_file_name = f;
}

AST_Expression::ExprComb
AST_Expression::ec ()
{
  return this->pd_ec;
}

AST_Expression::AST_ExprValue   *
AST_Expression::ev ()
{
  return this->pd_ev;
}

void
AST_Expression::set_ev (AST_Expression::AST_ExprValue *new_ev)
{
  delete this->pd_ev;
  this->pd_ev = new_ev;
}

AST_Expression *
AST_Expression::v1 ()
{
  return this->pd_v1;
}

void
AST_Expression::set_v1 (AST_Expression *e)
{
  this->pd_v1 = e;
}

AST_Expression *
AST_Expression::v2 ()
{
  return this->pd_v2;
}

void
AST_Expression::set_v2 (AST_Expression *e)
{
  this->pd_v2 = e;
}

UTL_ScopedName *
AST_Expression::n ()
{
  return this->pd_n;
}

void
AST_Expression::set_n (UTL_ScopedName *new_n)
{
  this->pd_n = new_n;
}

const char *
AST_Expression::exprtype_to_string (ExprType t)
{
  switch (t) {
  case AST_Expression::EV_short:
    return "short";
  case AST_Expression::EV_ushort:
    return "unsigned short";
  case AST_Expression::EV_long:
    return "long";
  case AST_Expression::EV_ulong:
    return "unsigned long";
  case AST_Expression::EV_float:
    return "float";
  case AST_Expression::EV_double:
    return "double";
  case AST_Expression::EV_char:
    return "char";
  case AST_Expression::EV_octet:
    return "octet";
  case AST_Expression::EV_bool:
    return "boolean";
  case AST_Expression::EV_string:
    return "string";
  case AST_Expression::EV_enum:
    return "enum";
  case AST_Expression::EV_void:
    return "void";
  case AST_Expression::EV_none:
    return "none";
  case AST_Expression::EV_wchar:
    return "wchar";
  case AST_Expression::EV_longlong:
    return "longlong";
  case AST_Expression::EV_ulonglong:
    return "ulonglong";
  case AST_Expression::EV_longdouble:
    return "longdouble";
  case AST_Expression::EV_wstring:
    return "wstring";
  case AST_Expression::EV_any:
    return "any";
  case AST_Expression::EV_object:
    return "object";
  case AST_Expression::EV_fixed:
    return "fixed";
  case AST_Expression::EV_uint8:
    return "uint8";
  case AST_Expression::EV_int8:
    return "int8";
  }

  return "<UNKNOWN TYPE>";
}

AST_Enum *
AST_Expression::enum_parent ()
{
  return enum_parent_;
}

void
AST_Expression::enum_parent (AST_Enum *node)
{
  enum_parent_ = node;
}
