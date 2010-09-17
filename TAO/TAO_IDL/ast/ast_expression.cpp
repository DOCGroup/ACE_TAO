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

// AST_Expression nodes denote IDL expressions used in the IDL input.

#include "ast_expression.h"
#include "ast_constant.h"
#include "ast_typedef.h"
#include "ast_param_holder.h"
#include "ast_visitor.h"
#include "ast_generator.h"

#include "utl_err.h"
#include "utl_scope.h"
#include "utl_string.h"

#include "nr_extern.h"
#include "global_extern.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

// Helper function to fill out the details of where this expression
// is defined.
void
AST_Expression::fill_definition_details (void)
{
  this->pd_defined_in = idl_global->scopes ().depth () > 0
                          ? idl_global->scopes().top ()
                          : 0 ;
  this->pd_line = idl_global->lineno ();
  this->pd_file_name = idl_global->filename ();
}

// An AST_Expression denoting a symbolic name.
AST_Expression::AST_Expression (UTL_ScopedName *nm)
  : pd_ec (EC_symbol),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (nm),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();

  AST_Decl *d =
    idl_global->scopes ().top_non_null ()->lookup_by_name (nm,
                                                           true);

  if (d->node_type () == AST_Decl::NT_param_holder)
    {
      this->param_holder_ =
        AST_Param_Holder::narrow_from_decl (d);
    }
}

// An AST_Expression denoting a type coercion from another AST_Expression.
AST_Expression::AST_Expression (AST_Expression *v,
                                ExprType t)
  : pd_ec (EC_none),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  AST_Param_Holder *ph = v->param_holder_;
  this->fill_definition_details ();

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
      if (ph == 0)
        {
          this->pd_ev = v->coerce (t);

          if (this->pd_ev == 0)
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

      if (0 != v->pd_n)
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
    pd_ev (0),
    pd_v1 (ev1),
    pd_v2 (ev2),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();
}

// An AST_Expression denoting a short integer.
AST_Expression::AST_Expression (ACE_CDR::Short sv)
  : pd_ec (EC_none),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_ulonglong;
#if defined (ACE_LACKS_LONGLONG_T)
  this->pd_ev->u.ulval = ulv;
#else
  this->pd_ev->u.ullval = ulv;
#endif
}

// An AST_Expression denoting an unsigned long integer.
AST_Expression::AST_Expression (ACE_CDR::ULong ulv,
                                ExprType t)
  : pd_ec (EC_none),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = t;
#if defined (ACE_LACKS_LONGLONG_T)
  this->pd_ev->u.ulval = ulv;
#else
  this->pd_ev->u.ullval = ulv;
#endif
}

// An AST_Expression denoting a 32-bit floating point number.
AST_Expression::AST_Expression (ACE_CDR::Float fv)
  : pd_ec (EC_none),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
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
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  UTL_String *new_str = 0;
  ACE_NEW (new_str,
           UTL_String (sv, true));

  this->pd_ev->u.strval = new_str;
  this->pd_ev->et = EV_string;
}

// An AST_Expression denoting a wide string.
AST_Expression::AST_Expression (char *sv)
  : pd_ec (EC_none),
    pd_ev (0),
    pd_v1 (0),
    pd_v2 (0),
    pd_n (0),
    tdef (0),
    param_holder_ (0)
{
  this->fill_definition_details ();

  ACE_NEW (this->pd_ev,
           AST_ExprValue);

  this->pd_ev->et = EV_wstring;
  this->pd_ev->u.wstrval = sv;
}

AST_Expression::~AST_Expression (void)
{
}

AST_Expression::AST_ExprValue::AST_ExprValue (void)
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
  if (ev == 0)
    {
      return 0;
    }

  switch (t)
    {
    case AST_Expression::EV_short:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          return ev;
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (unsigned short) ACE_INT16_MAX)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.usval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval > (long) ACE_INT16_MAX
              || ev->u.lval < (long) ACE_INT16_MIN)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.lval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (unsigned long) ACE_INT16_MAX)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.ulval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_INT16_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_INT16_MIN)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.llval;
          ev->et = AST_Expression::EV_short;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_INT16_MAX) != ev->u.ullval)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.ullval;
          ev->et = AST_Expression::EV_short;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.sval = (short) ev->u.bval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval > (float) ACE_INT16_MAX
              || ev->u.fval < (float) ACE_INT16_MIN)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.fval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > (double) ACE_INT16_MAX
              || ev->u.dval < (double) ACE_INT16_MIN)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.dval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_char:
          ev->u.sval = (short) ev->u.cval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_INT16_MAX)
            {
              return 0;
            }

          ev->u.sval = (short) ev->u.wcval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.sval = (short) ev->u.oval;
          ev->et = AST_Expression::EV_short;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_string:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_ushort:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.sval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_ushort:
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval > (long) ACE_UINT16_MAX
              || ev->u.lval < 0)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.lval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (unsigned long) ACE_UINT16_MAX)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.ulval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_UINT16_MAX
              || ev->u.llval < 0)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.llval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_UINT16_MAX) != ev->u.ullval)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.ullval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.usval = (unsigned short) ev->u.bval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (float) ACE_UINT16_MAX)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.fval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (double) ACE_UINT16_MAX)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.dval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
                    return 0;
            }

          ev->u.usval = (unsigned short) ev->u.cval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.usval = (unsigned short) ev->u.wcval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.usval = (unsigned short) ev->u.oval;
          ev->et = AST_Expression::EV_ushort;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_long:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          ev->u.lval = (long) ev->u.sval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.lval = (long) ev->u.usval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_long:
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (unsigned long) ACE_INT32_MAX)
            {
                    return 0;
            }

          ev->u.lval = (long) ev->u.ulval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_INT32_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_INT32_MIN)
            {
                    return 0;
            }
          ev->u.lval = (long) ev->u.llval;
          ev->et = AST_Expression::EV_long;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_INT32_MAX) != ev->u.ullval)
            {
                    return 0;
            }

          ev->u.lval = (long) ev->u.ullval;
          ev->et = AST_Expression::EV_long;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.lval = (long) ev->u.bval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval > (float) LONG_MAX
              || ev->u.fval < (float) ACE_INT32_MIN)
            {
                    return 0;
            }

          ev->u.lval = (long) ev->u.fval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > (double) LONG_MAX
              || ev->u.dval < (double) ACE_INT32_MIN)
            {
                    return 0;
            }

          ev->u.lval = (long) ev->u.dval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_char:
          ev->u.lval = (long) ev->u.cval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.lval = (long) ev->u.wcval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.lval = (long) ev->u.oval;
          ev->et = AST_Expression::EV_long;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_ulong:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.sval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.ulval = (unsigned long) ev->u.usval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.lval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_ulong:
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_UINT32_MAX
              || ev->u.llval < 0)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.llval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_UINT32_MAX) != ev->u.ullval)
            {
                    return 0;
            }
          ev->u.ulval = (unsigned long) ev->u.ullval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.ulval = (unsigned long) ev->u.bval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (float) ACE_UINT32_MAX)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.fval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (double) ACE_UINT32_MAX)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.dval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
                    return 0;
            }

          ev->u.ulval = (unsigned long) ev->u.cval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.ulval = (unsigned long) ev->u.wcval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.ulval = (unsigned long) ev->u.oval;
          ev->et = AST_Expression::EV_ulong;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.sval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.usval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_long:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.lval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_ulong:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.ulval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_longlong:
          return ev;
        case AST_Expression::EV_ulonglong:
          if (ev->u.ullval > ACE_INT64_MAX)
            {
                    return 0;
            }

          ev->u.llval =
            static_cast<ACE_CDR::LongLong> (ev->u.ullval);
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_bool:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.bval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval > (float) ACE_INT64_MAX
              || ev->u.fval < (float) ACE_INT64_MIN)
            {
                    return 0;
            }

          ev->u.llval = (ACE_CDR::LongLong) ev->u.fval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > (double) ACE_INT64_MAX
              || ev->u.dval < (double) ACE_INT64_MIN)
            {
                    return 0;
            }

          ev->u.llval = (ACE_CDR::LongLong) ev->u.dval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_char:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.cval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.wcval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.llval = (ACE_CDR::LongLong) ev->u.oval;
          ev->et = AST_Expression::EV_longlong;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
                    return 0;
            }

          ev->u.ullval =  ev->u.sval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.ullval = ev->u.usval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0)
            {
                    return 0;
            }

          ev->u.ullval = ev->u.lval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_ulong:
          ev->u.ullval = ev->u.ulval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_longlong:
          if (ev->u.llval < 0)
            {
                    return 0;
            }

          ev->u.ullval =
            static_cast<ACE_CDR::LongLong> (ev->u.llval);
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_ulonglong:
          return ev;
        case AST_Expression::EV_bool:
          ev->u.ullval = ev->u.bval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_float:
#if defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
          if (ev->u.fval < 0.0
              || ev->u.fval > (float) ACE_INT64_MAX)
            {
                    return 0;
            }

          ev->u.ullval = static_cast<ACE_UINT32> (ev->u.fval);
#else
          if (ev->u.fval < 0.0
              || ev->u.fval > (float) ACE_UINT64_MAX)
            {
                    return 0;
            }

          ev->u.ullval = static_cast<ACE_UINT64> (ev->u.fval);
#endif

          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_double:
#if defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
          if (ev->u.dval < 0.0
              || ev->u.dval > (double) ACE_INT64_MAX)
            {
                    return 0;
            }

          ev->u.ullval = static_cast<ACE_UINT32> (ev->u.dval);
#else
          if (ev->u.dval < 0.0
              || ev->u.dval > (double) ACE_UINT64_MAX)
            {
                    return 0;
            }

          ev->u.ullval = static_cast<ACE_UINT64> (ev->u.dval);
#endif

          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
                    return 0;
            }

          ev->u.ullval = ev->u.cval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.ullval = ev->u.wcval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.ullval = ev->u.oval;
          ev->et = AST_Expression::EV_ulonglong;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case AST_Expression::EV_bool:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          ev->u.bval = (ev->u.sval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.bval = (ev->u.usval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_long:
          ev->u.bval = (ev->u.lval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_ulong:
          ev->u.bval = (ev->u.ulval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          ev->u.bval = (ev->u.llval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
       case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          ev->u.bval = (ev->u.ullval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          return ev;
        case AST_Expression::EV_float:
          ev->u.bval = (ev->u.fval == 0.0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_double:
          ev->u.bval = (ev->u.dval == 0.0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_char:
          ev->u.bval = (ev->u.cval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.bval = (ev->u.wcval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.bval = (ev->u.oval == 0) ? false : true;
          ev->et = AST_Expression::EV_bool;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
         return 0;
        }
    case AST_Expression::EV_float:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          ev->u.fval = (float) ev->u.sval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.fval = (float) ev->u.usval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_long:
          ev->u.fval = (float) ev->u.lval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_ulong:
          ev->u.fval = (float) ev->u.ulval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > FLT_MAX
              || ev->u.llval < -(ACE_FLT_MAX))
            {
              return 0;
            }
          ev->u.fval = (float) ev->u.llval;
          ev->et = AST_Expression::EV_float;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          ev->u.fval = (float) ((ACE_CDR::LongLong) ev->u.ullval);
          ev->et = AST_Expression::EV_float;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.fval = (float) ((ev->u.bval == true) ? 1.0 : 0.0);
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_float:
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > ACE_FLT_MAX
              || ev->u.dval < -(ACE_FLT_MAX))
            {
                    return 0;
            }

          ev->u.fval = (float) ev->u.dval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_char:
          ev->u.fval = (float) ev->u.cval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.fval = (float) ev->u.wcval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.fval = (float) ev->u.oval;
          ev->et = AST_Expression::EV_float;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_double:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          ev->u.dval = (double) ev->u.sval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.dval = (double) ev->u.usval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_long:
          ev->u.dval = (double) ev->u.lval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_ulong:
          ev->u.dval = (double) ev->u.ulval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          ev->u.dval = (double) ev->u.llval;
          ev->et = AST_Expression::EV_double;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          // Some compilers don't implement unsigned 64-bit to double
          // conversions, so we are stuck with the signed 64-bit max value.
          if (ev->u.ullval > ACE_INT64_MAX)
            {
              return 0;
            }

          ev->u.dval = (double) ((ACE_CDR::LongLong) ev->u.ullval);
          ev->et = AST_Expression::EV_double;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.dval = (ev->u.bval == true) ? 1.0 : 0.0;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_float:
          ev->u.dval = (double) ev->u.fval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_double:
          return ev;
        case AST_Expression::EV_char:
          ev->u.dval = (double) ev->u.cval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_wchar:
          ev->u.dval = (double) ev->u.wcval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_octet:
          ev->u.dval = (double) ev->u.oval;
          ev->et = AST_Expression::EV_double;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_char:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval > (short) ACE_CHAR_MAX
              || ev->u.sval < (short) ACE_CHAR_MIN)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.sval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (unsigned short) ACE_CHAR_MAX)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.usval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval > (long) ACE_CHAR_MAX
              || ev->u.lval < (long) ACE_CHAR_MIN)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.lval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (unsigned long) ACE_CHAR_MAX)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.ulval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_CHAR_MAX
              || ev->u.llval < (ACE_CDR::LongLong) ACE_CHAR_MIN)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.llval;
          ev->et = AST_Expression::EV_char;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
       case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (( ev->u.ullval & ACE_CHAR_MAX) != ev->u.ullval)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.ullval;
          ev->et = AST_Expression::EV_char;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.cval = (char) ev->u.bval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval > (float) ACE_CHAR_MAX
              || ev->u.fval < (float) ACE_CHAR_MIN)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.fval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > (double) ACE_CHAR_MAX
              || ev->u.dval < (double) ACE_CHAR_MIN)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.dval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_char:
          return ev;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_CHAR_MAX)
            {
              return 0;
            }

          ev->u.cval = (char) ev->u.wcval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_octet:
          if (ev->u.oval > (unsigned char) ACE_CHAR_MAX)
            {
                    return 0;
            }

          ev->u.cval = (char) ev->u.oval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_wchar:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0)
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.sval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_ushort:
          ev->u.wcval = (ACE_CDR::WChar) ev->u.usval;
          ev->et = AST_Expression::EV_char;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0
              || ev->u.lval > ACE_WCHAR_MAX)
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.lval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > ACE_WCHAR_MAX)
            {
              return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.ulval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval > (ACE_CDR::LongLong) ACE_WCHAR_MAX
              || ev->u.llval < 0)
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.llval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_WCHAR_MAX) != ev->u.ullval )
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.ullval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.wcval = (ACE_CDR::WChar) ev->u.bval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval > (float) ACE_WCHAR_MAX
              || ev->u.fval < 0)
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.fval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval > (double) ACE_WCHAR_MAX
              || ev->u.dval < 0)
            {
                    return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.dval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
              return 0;
            }

          ev->u.wcval = (ACE_CDR::WChar) ev->u.cval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_wchar:
          return ev;
        case AST_Expression::EV_octet:
          ev->u.wcval = (ACE_CDR::WChar) ev->u.oval;
          ev->et = AST_Expression::EV_wchar;
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_octet:
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          if (ev->u.sval < 0
              || ev->u.sval > (short) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.sval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_ushort:
          if (ev->u.usval > (unsigned short) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.usval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_long:
          if (ev->u.lval < 0
              || ev->u.lval > (long) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.lval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_ulong:
          if (ev->u.ulval > (unsigned long) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.ulval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if (ev->u.llval < 0
              || ev->u.llval > (ACE_CDR::LongLong) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.llval;
          ev->et = AST_Expression::EV_octet;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
          if ((ev->u.ullval & ACE_OCTET_MAX) != ev->u.ullval)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.ullval;
          ev->et = AST_Expression::EV_octet;
          return ev;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
          return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case AST_Expression::EV_bool:
          ev->u.oval = (unsigned char) ((ev->u.bval == false) ? 1 : 0);
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_float:
          if (ev->u.fval < 0.0
              || ev->u.fval > (float) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.fval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_double:
          if (ev->u.dval < 0.0
              || ev->u.dval > (double) ACE_OCTET_MAX)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.dval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_char:
          if ((signed char) ev->u.cval < 0)
            {
                    return 0;
            }

          ev->u.oval = (unsigned char) ev->u.cval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_wchar:
          if (ev->u.wcval > (ACE_CDR::WChar) ACE_OCTET_MAX)
            {
              return 0;
            }

          ev->u.oval = (unsigned char) ev->u.wcval;
          ev->et = AST_Expression::EV_octet;
          return ev;
        case AST_Expression::EV_octet:
          return ev;
        case AST_Expression::EV_longdouble:
        case AST_Expression::EV_wstring:
        case AST_Expression::EV_string:
        case AST_Expression::EV_enum:
        case AST_Expression::EV_void:
        case AST_Expression::EV_none:
        case AST_Expression::EV_any:
        case AST_Expression::EV_object:
          return 0;
        }
    case AST_Expression::EV_enum:
      switch (ev->et)
        {
        case AST_Expression::EV_enum:
        case AST_Expression::EV_ulong:
          return ev;
        default:
          return 0;
        }
    case AST_Expression::EV_void:
      switch (ev->et)
        {
        case AST_Expression::EV_void:
          return ev;
        default:
          return 0;
        }
    case AST_Expression::EV_none:
      return 0;
    case AST_Expression::EV_string:
      switch (ev->et)
        {
        case AST_Expression::EV_string:
          return ev;
        default:
          return 0;
        }
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_any:
    case AST_Expression::EV_object:
      return 0;
    }

  return 0;
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
          return 0;
        default:
          return 1;
      }
    case AST_Expression::EV_float:
    case AST_Expression::EV_double:
    case AST_Expression::EV_longdouble:
      switch (t2)
      {
        case AST_Expression::EV_float:
        case AST_Expression::EV_double:
        case AST_Expression::EV_longdouble:
          return 0;
        default:
          return 1;
      }
    case AST_Expression::EV_char:
    case AST_Expression::EV_wchar:
    case AST_Expression::EV_string:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_enum:
    case AST_Expression::EV_any:
    case AST_Expression::EV_object:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
    default:
      return 0;
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
  AST_Expression::AST_ExprValue *newval = 0;
  ACE_NEW_RETURN (newval,
                  AST_Expression::AST_ExprValue,
                  0);

  if (ev != 0)
    {
      *newval = *ev;
    }

  AST_Expression::AST_ExprValue *retval = 0;

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
#if ! defined (ACE_LACKS_LONGLONG_T)
    case AST_Expression::EK_longlong:
      retval = coerce_value (newval, AST_Expression::EV_longlong);
      break;
    case AST_Expression::EK_ulonglong:
      retval = coerce_value (newval, AST_Expression::EV_ulonglong);
      break;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case AST_Expression::EK_octet:
      retval = coerce_value (newval, AST_Expression::EV_octet);
      break;
    case AST_Expression::EK_bool:
      retval = coerce_value (newval, AST_Expression::EV_bool);
      break;
    default:
      break;
  }

  // Sometimes the call above to coerce_value() will return an
  // evaluated newval, other times 0. But a heap-allocated
  // ExprValue is not always passed to coerce_value(), so we
  // have to manage it here, where we know it is always a 'new'.
  if (retval != newval)
    {
      delete newval;
      newval = 0;
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
AST_Expression::AST_ExprValue *
AST_Expression::eval_bin_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = 0;

  if (this->pd_v1 == 0 || this->pd_v2 == 0)
    {
      return 0;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == 0 || this->pd_v2->ev () == 0)
    {
      return 0;
    }

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  0);

#if !defined (ACE_LACKS_LONGLONG_T)
  if (ek == EK_ulonglong)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ulonglong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ulonglong));
      retval->et = EV_ulonglong;

      switch (this->pd_ec)
  {
  case EC_add:
    retval->u.ullval =
      this->pd_v1->ev ()->u.ullval + this->pd_v2->ev ()->u.ullval;
    break;
  case EC_minus:
    retval->u.ullval =
      this->pd_v1->ev ()->u.ullval - this->pd_v2->ev ()->u.ullval;
    break;
  case EC_mul:
    retval->u.ullval =
      this->pd_v1->ev ()->u.ullval * this->pd_v2->ev ()->u.ullval;
    break;
  case EC_div:
    if (this->pd_v2->ev ()->u.ullval == 0)
      {
        return 0;
      }

    retval->u.ullval =
      this->pd_v1->ev ()->u.ullval / this->pd_v2->ev  ()->u.ullval;
    break;
  default:
    return 0;
  }
    }
  else if (ek == EK_longlong)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_longlong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_longlong));
      retval->et = EV_longlong;

      switch (this->pd_ec)
  {
  case EC_add:
    retval->u.llval =
      this->pd_v1->ev ()->u.llval + this->pd_v2->ev ()->u.llval;
    break;
  case EC_minus:
    retval->u.llval =
      this->pd_v1->ev ()->u.llval - this->pd_v2->ev ()->u.llval;
    break;
  case EC_mul:
    retval->u.llval =
      this->pd_v1->ev ()->u.llval * this->pd_v2->ev ()->u.llval;
    break;
  case EC_div:
    if (this->pd_v2->ev ()->u.llval == 0)
      {
        return 0;
      }

    retval->u.llval =
      this->pd_v1->ev ()->u.llval / this->pd_v2->ev  ()->u.llval;
    break;
  default:
    return 0;
  }
    }
  else
#endif
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_double));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_double));
      retval->et = EV_double;

      switch (this->pd_ec)
  {
  case EC_add:
    retval->u.dval =
      this->pd_v1->ev ()->u.dval + this->pd_v2->ev ()->u.dval;
    break;
  case EC_minus:
    retval->u.dval =
      this->pd_v1->ev ()->u.dval - this->pd_v2->ev ()->u.dval;
    break;
  case EC_mul:
    retval->u.dval =
      this->pd_v1->ev ()->u.dval * this->pd_v2->ev ()->u.dval;
    break;
  case EC_div:
    if (this->pd_v2->ev ()->u.dval == 0.0)
      {
        return 0;
      }

    retval->u.dval =
      this->pd_v1->ev ()->u.dval / this->pd_v2->ev  ()->u.dval;
    break;
  default:
    return 0;
  }
    }

  return retval;
}
// Apply binary operators to an AST_Expression after evaluating
// its sub-expressions.
// Operations supported: '%'
AST_Expression::AST_ExprValue *
AST_Expression::eval_mod_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = 0;

  if (this->pd_v1 == 0 || this->pd_v2 == 0)
    {
      return 0;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == 0 || this->pd_v2->ev () == 0)
    {
      return 0;
    }

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  0);

#if !defined (ACE_LACKS_LONGLONG_T)
  if (ek == EK_ulonglong)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ulonglong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ulonglong));
      retval->et = EV_ulonglong;

      if (this->pd_v2->ev ()->u.ullval == 0)
        {
          return 0;
        }

      retval->u.ullval =
        this->pd_v1->ev ()->u.ullval % this->pd_v2->ev ()->u.ullval;
    }
  else if (ek == EK_longlong)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_longlong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_longlong));
      retval->et = EV_longlong;

      if (this->pd_v2->ev ()->u.llval == 0)
        {
          return 0;
        }

      retval->u.llval =
        this->pd_v1->ev ()->u.llval % this->pd_v2->ev ()->u.llval;
    }
  else
#endif
  if (ek == EK_ulong)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ulong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ulong));
      retval->et = EV_ulong;

      if (this->pd_v2->ev ()->u.ulval == 0)
        {
          return 0;
        }

      retval->u.ulval =
        this->pd_v1->ev ()->u.ulval % this->pd_v2->ev ()->u.ulval;
    }
  else if (ek == EK_long)
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_long));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_long));
      retval->et = EV_long;

      if (this->pd_v2->ev ()->u.lval == 0)
        {
          return 0;
        }

      retval->u.lval =
        this->pd_v1->ev ()->u.lval % this->pd_v2->ev ()->u.lval;
    }
  else
    {
      return 0;
    }

  return retval;
}

// Apply bitwise operations to an AST_Expression after evaluating
// its sub-expressions.
// Operations supported: '%', '|', '&', '^', '<<', '>>'
AST_Expression::AST_ExprValue *
AST_Expression::eval_bit_op (AST_Expression::EvalKind ek)
{
  AST_Expression::AST_ExprValue *retval = 0;

  if (this->pd_v1 == 0 || this->pd_v2 == 0)
    {
      return 0;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));
  this->pd_v2->set_ev (this->pd_v2->eval_internal (ek));

  if (this->pd_v1->ev () == 0 || this->pd_v2->ev () == 0 )
    {
      return 0;
    }

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  0);

  switch (ek)
  {
#if !defined (ACE_LACKS_LONGLONG_T)
  case EK_ulonglong:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ulonglong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ulonglong));
      retval->et = EV_ulonglong;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.ullval =
            this->pd_v1->ev ()->u.ullval | this->pd_v2->ev ()->u.ullval;
          break;
        case EC_xor:
          retval->u.ullval =
            this->pd_v1->ev ()->u.ullval ^ this->pd_v2->ev ()->u.ullval;
          break;
        case EC_and:
          retval->u.ullval =
            this->pd_v1->ev ()->u.ullval & this->pd_v2->ev ()->u.ullval;
          break;
        case EC_left:
          retval->u.ullval =
            this->pd_v1->ev ()->u.ullval << this->pd_v2->ev ()->u.ullval;
          break;
        case EC_right:
          retval->u.ullval =
            this->pd_v1->ev ()->u.ullval >> this->pd_v2->ev ()->u.ullval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_longlong:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_longlong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_longlong));
      retval->et = EV_longlong;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.llval =
            this->pd_v1->ev ()->u.llval | this->pd_v2->ev ()->u.llval;
          break;
        case EC_xor:
          retval->u.llval =
            this->pd_v1->ev ()->u.llval ^ this->pd_v2->ev ()->u.llval;
          break;
        case EC_and:
          retval->u.llval =
            this->pd_v1->ev ()->u.llval & this->pd_v2->ev ()->u.llval;
          break;
        case EC_left:
          retval->u.llval =
            this->pd_v1->ev ()->u.llval << this->pd_v2->ev ()->u.llval;
          break;
        case EC_right:
          retval->u.llval =
            this->pd_v1->ev ()->u.llval >> this->pd_v2->ev ()->u.llval;
          break;
        default:
          return 0;
      }
    }

    break;
#endif /* !defined (ACE_LACKS_LONGLONG_T) */
  case EK_ulong:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ulong));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ulong));
      retval->et = EV_ulong;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.ulval =
            this->pd_v1->ev ()->u.ulval | this->pd_v2->ev ()->u.ulval;
          break;
        case EC_xor:
          retval->u.ulval =
            this->pd_v1->ev ()->u.ulval ^ this->pd_v2->ev ()->u.ulval;
          break;
        case EC_and:
          retval->u.ulval =
            this->pd_v1->ev ()->u.ulval & this->pd_v2->ev ()->u.ulval;
          break;
        case EC_left:
          retval->u.ulval =
            this->pd_v1->ev ()->u.ulval << this->pd_v2->ev ()->u.ulval;
          break;
        case EC_right:
          retval->u.ulval =
            this->pd_v1->ev ()->u.ulval >> this->pd_v2->ev ()->u.ulval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_long:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_long));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_long));
      retval->et = EV_long;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.lval =
            this->pd_v1->ev ()->u.lval | this->pd_v2->ev ()->u.lval;
          break;
        case EC_xor:
          retval->u.lval =
            this->pd_v1->ev ()->u.lval ^ this->pd_v2->ev ()->u.lval;
          break;
        case EC_and:
          retval->u.lval =
            this->pd_v1->ev ()->u.lval & this->pd_v2->ev ()->u.lval;
          break;
        case EC_left:
          retval->u.lval =
            this->pd_v1->ev ()->u.lval << this->pd_v2->ev ()->u.lval;
          break;
        case EC_right:
          retval->u.lval =
            this->pd_v1->ev ()->u.lval >> this->pd_v2->ev ()->u.lval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_ushort:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_ushort));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_ushort));
      retval->et = EV_ushort;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.usval =
            this->pd_v1->ev ()->u.usval | this->pd_v2->ev ()->u.usval;
          break;
        case EC_xor:
          retval->u.usval =
            this->pd_v1->ev ()->u.usval ^ this->pd_v2->ev ()->u.usval;
          break;
        case EC_and:
          retval->u.usval =
            this->pd_v1->ev ()->u.usval & this->pd_v2->ev ()->u.usval;
          break;
        case EC_left:
          retval->u.usval =
            this->pd_v1->ev ()->u.usval << this->pd_v2->ev ()->u.usval;
          break;
        case EC_right:
          retval->u.usval =
            this->pd_v1->ev ()->u.usval >> this->pd_v2->ev ()->u.usval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_short:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_short));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_short));
      retval->et = EV_short;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.sval =
            this->pd_v1->ev ()->u.sval | this->pd_v2->ev ()->u.sval;
          break;
        case EC_xor:
          retval->u.sval =
            this->pd_v1->ev ()->u.sval ^ this->pd_v2->ev ()->u.sval;
          break;
        case EC_and:
          retval->u.sval =
            this->pd_v1->ev ()->u.sval & this->pd_v2->ev ()->u.sval;
          break;
        case EC_left:
          retval->u.sval =
            this->pd_v1->ev ()->u.sval << this->pd_v2->ev ()->u.sval;
          break;
        case EC_right:
          retval->u.sval =
            this->pd_v1->ev ()->u.sval >> this->pd_v2->ev ()->u.sval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_bool:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_bool));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_bool));
      retval->et = EV_bool;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.bval =
            this->pd_v1->ev ()->u.bval | this->pd_v2->ev ()->u.bval;
          break;
        case EC_xor:
          retval->u.bval =
            this->pd_v1->ev ()->u.bval ^ this->pd_v2->ev ()->u.bval;
          break;
        case EC_and:
          retval->u.bval =
            this->pd_v1->ev ()->u.bval & this->pd_v2->ev ()->u.bval;
          break;
        case EC_left:
          retval->u.bval =
            this->pd_v1->ev ()->u.ulval << this->pd_v2->ev ()->u.ulval;
          break;
        case EC_right:
          retval->u.bval =
            this->pd_v1->ev ()->u.ulval >> this->pd_v2->ev ()->u.ulval;
          break;
        default:
          return 0;
      }
    }

    break;
  case EK_octet:
    {
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_octet));
      this->pd_v2->set_ev (this->pd_v2->coerce (EV_octet));
      retval->et = EV_octet;

      switch (this->pd_ec)
      {
        case EC_or:
          retval->u.oval =
            this->pd_v1->ev ()->u.oval | this->pd_v2->ev ()->u.oval;
          break;
        case EC_xor:
          retval->u.oval =
            this->pd_v1->ev ()->u.oval ^ this->pd_v2->ev ()->u.oval;
          break;
        case EC_and:
          retval->u.oval =
            this->pd_v1->ev ()->u.oval & this->pd_v2->ev ()->u.oval;
          break;
        case EC_left:
          {
            // This is the only bitwise operation that can cause overflow
            // even if both operands are in range, so we set the ExprType
            // to a large type and then check for overflow.
            retval->u.ulval =
              this->pd_v1->ev ()->u.ulval << this->pd_v2->ev ()->u.ulval;
            AST_Expression test (retval->u.ulval, EV_ulong);
            AST_ExprValue *ev = test.coerce (EV_octet);
            delete retval;
            retval = ev;
            break;
          }
        case EC_right:
          retval->u.oval =
            this->pd_v1->ev ()->u.oval >> this->pd_v2->ev ()->u.oval;
          break;
        default:
          return 0;
      }
    }

    break;
  default:
    return 0;
  }

  return retval;
}

// Apply unary operators to an AST_Expression after evaluating its
// sub-expression.
// Operations supported: '-', '+', '~'
AST_Expression::AST_ExprValue *
AST_Expression::eval_un_op (AST_Expression::EvalKind ek)
{
  AST_ExprValue *retval = 0;

  if (this->pd_ev != 0)
    {
      return this->pd_ev;
    }

  if (this->pd_v1 == 0)
    {
      return 0;
    }

  this->pd_v1->set_ev (this->pd_v1->eval_internal (ek));

  if (this->pd_v1->ev () == 0)
    {
      return 0;
    }

  ACE_NEW_RETURN (retval,
                  AST_ExprValue,
                  0);

  retval->et = EV_double;

  switch (this->pd_ec)
    {
    case EC_u_plus:
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_double));

      if (this->pd_v1->ev () == 0)
        {
          return 0;
        }

      retval->u.dval = this->pd_v1->ev ()->u.dval;
      break;
    case EC_u_minus:
      this->pd_v1->set_ev (this->pd_v1->coerce (EV_double));

      if (this->pd_v1->ev () == 0)
        {
          return 0;
        }

      retval->u.dval = -(this->pd_v1->ev ()->u.dval);
      break;
    case EC_bit_neg:
      if (this->pd_v1->ev () == 0)
        {
          return 0;
        }

      switch (this->pd_v1->ev ()->et)
      {
        case EV_short:
          retval->et = EV_short;
          retval->u.sval = ~this->pd_v1->ev ()->u.sval;
          break;
        case EV_ushort:
          retval->et = EV_ushort;
          retval->u.usval = ~this->pd_v1->ev ()->u.usval;
          break;
        case EV_long:
          retval->et = EV_long;
          retval->u.lval = ~this->pd_v1->ev ()->u.lval;
          break;
        case EV_ulong:
          retval->et = EV_ulong;
          retval->u.ulval = ~this->pd_v1->ev ()->u.ulval;
          break;
#if ! defined (ACE_LACKS_LONGLONG_T)
        case EV_longlong:
          retval->et = EV_longlong;
          retval->u.llval = ~this->pd_v1->ev ()->u.llval;
          break;
        case EV_ulonglong:
          retval->et = EV_ulonglong;
          retval->u.ullval = ~this->pd_v1->ev ()->u.ullval;
          break;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        case EV_octet:
          retval->et = EV_octet;
          retval->u.oval = ~this->pd_v1->ev ()->u.oval;
          break;
        default:
          return 0;
      }

      break;
    default:
      return 0;
  }

  return retval;
}

// Evaluate a symbolic AST_Expression by looking up the named
// symbol.
AST_Expression::AST_ExprValue *
AST_Expression::eval_symbol (AST_Expression::EvalKind ek)
{
  UTL_Scope     *s = 0;
  AST_Decl *d = 0;
  AST_Constant *c = 0;

  // Is there a symbol stored?
  if (this->pd_n == 0)
    {
      idl_global->err ()->eval_error  (this);
      return 0;
    }

  // Get current scope for lookup.
  if (idl_global->scopes ().depth () > 0)
    {
      s = idl_global->scopes ().top_non_null ();
    }

  if (s == 0)
    {
      idl_global->err ()->lookup_error (this->pd_n);
      return 0;
    }

  // If we are a template parameter placeholder, just skip the
  // rest - nothing needs to be evaluated until instantiation
  // time.
  if (this->param_holder_ != 0)
    {
      return 0;
    }

  // Do lookup.
  d = s->lookup_by_name (this->pd_n,
                         true);

  if (d == 0)
    {
      idl_global->err ()->lookup_error (this->pd_n);
      return 0;
    }

  // Is it a constant?
  if (d->node_type () != AST_Decl::NT_const
      && d->node_type () != AST_Decl::NT_enum_val)
    {
      idl_global->err ()->constant_expected (this->pd_n,
                                             d);
      return 0;
    }

  // OK, now evaluate the constant we just got, to produce its value.
  c = AST_Constant::narrow_from_decl (d);

  if (c == 0)
    {
      return 0;
    }

  return c->constant_value ()->eval_internal (ek);
}

bool
AST_Expression::type_mismatch (AST_Expression::ExprType t)
{
  if (this->pd_ev != 0)
    {
      return incompatible_types (this->pd_ev->et, t);
    }

  bool v1_mismatch = 0;
  bool v2_mismatch = 0;

  if (this->pd_v1 != 0)
    {
      v1_mismatch = this->pd_v1->type_mismatch (t);
    }

  if (this->pd_v2 != 0)
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
  if (d != 0)
    {
      AST_Decl *enum_val =
        idl_global->scopes ().top_non_null ()->lookup_by_name (this->pd_n,
                                                               1);

      if (enum_val != 0)
        {
          AST_Decl *enum_decl = ScopeAsDecl (enum_val->defined_in ());

          if (d->node_type () == AST_Decl::NT_typedef)
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
              d = td->primitive_base_type ();
            }

          if (d != enum_decl)
            {
              idl_global->err ()->incompatible_type_error (this);
              return 0;
            }
        }
    }

  if (this->type_mismatch (t))
    {
      idl_global->err ()->incompatible_type_error (this);
      return 0;
    }

  if (d != 0 && d->node_type () == AST_Decl::NT_typedef)
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
  AST_ExprValue *tmp = 0;

  // First, evaluate it, then try to coerce result type.
  // If already evaluated, return the result.
  switch (t)
  {
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
#if ! defined (ACE_LACKS_LONGLONG_T)
    case EV_longlong:
      tmp = this->eval_internal (EK_longlong);
      break;
    case EV_ulonglong:
      tmp = this->eval_internal (EK_ulonglong);
      break;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_octet:
      tmp = this->eval_internal (EK_octet);
      break;
    case EV_bool:
      tmp = this->eval_internal (EK_bool);
      break;
    default:
      tmp = this->eval_internal (EK_const);
      break;
  }

  if (tmp == 0)
    {
      return 0;
    }
  else
    {
      delete this->pd_ev;
      this->pd_ev = tmp;
    }

  // Create a copy to contain coercion result.
  AST_ExprValue *copy = 0;
  ACE_NEW_RETURN (copy,
                  AST_ExprValue,
                  0);

  copy->et = this->pd_ev->et;

  switch (this->pd_ev->et)
  {
    case EV_longdouble:
    case EV_void:
    case EV_none:
      delete copy;
      return 0;
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
#if ! defined (ACE_LACKS_LONGLONG_T)
      copy->u.llval = this->pd_ev->u.llval;
      break;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      delete copy;
      return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      copy->u.ullval = this->pd_ev->u.ullval;
      break;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      delete copy;
      return 0;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
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
    default:
      break;
  }

  if (this->pd_ev->et == t)
    {
      return copy;
    }
  else
    {
      return coerce_value (copy, t);
    }
}

// Eval used internally.
AST_Expression::AST_ExprValue *
AST_Expression::eval_internal (AST_Expression::EvalKind ek)
{
  // Already evaluated?
  if (this->pd_ev != 0)
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
      return 0;
    }

  return 0;
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
  if (ek == AST_Expression::EK_const && this->pd_n != 0)
    {
      this->pd_ev->et = AST_Expression::EV_enum;
    }
}

// Expression equality comparison operator.
bool
AST_Expression::operator== (AST_Expression *vc)
{
  if (this->pd_ec != vc->ec ())
    {
      return false;
    }

  this->evaluate (EK_const);
  vc->evaluate (EK_const);

  if (pd_ev == 0 || vc->ev() == 0)
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
      return this->pd_ev->u.sval == vc->ev ()->u.sval ? true : false;
    case EV_ushort:
      return this->pd_ev->u.usval == vc->ev ()->u.usval ? true : false;
    case EV_long:
      return this->pd_ev->u.lval == vc->ev ()->u.lval ? true : false;
    case EV_ulong:
      return this->pd_ev->u.ulval == vc->ev()->u.ulval ? true : false;
    case EV_float:
      return this->pd_ev->u.fval == vc->ev ()->u.fval ? true : false;
    case EV_double:
      return this->pd_ev->u.dval == vc->ev ()->u.dval ? true : false;
    case EV_char:
      return this->pd_ev->u.cval == vc->ev ()->u.cval ? true : false;
    case EV_wchar:
      return this->pd_ev->u.wcval == vc->ev ()->u.wcval ? true : false;
    case EV_octet:
      return this->pd_ev->u.oval == vc->ev ()->u.oval ? true : false;
    case EV_bool:
      return this->pd_ev->u.lval == vc->ev ()->u.lval ? true : false;
    case EV_string:
      if (this->pd_ev->u.strval == 0)
        {
          if (vc->ev ()->u.strval == 0)
            {
                    return true;
            }
          else
            {
                    return false;
            }
        }
      else if (vc->ev ()->u.strval == 0)
        {
          return false;
        }
      else
        {
          return this->pd_ev->u.strval == vc->ev ()->u.strval
                   ? true
                   : false;
        }

    case EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      return pd_ev->u.llval == vc->ev ()->u.llval ? true : false;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return false;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      return pd_ev->u.ullval == vc->ev()->u.ullval ? true : false;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return false;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_longdouble:
    case EV_wstring:
    case EV_enum:
    case EV_void:
    case EV_none:
    case EV_any:
    case EV_object:
      return false;
    }

  return false;
}

long
AST_Expression::compare (AST_Expression *vc)
{
  if (this->pd_ec != vc->ec ())
    {
      return false;
    }

  this->evaluate (EK_const);
  vc->evaluate (EK_const);

  if (this->pd_ev == 0 || vc->ev () == 0)
    {
      return false;
    }

  if (this->pd_ev->et != vc->ev ()->et)
    {
      return false;
    }

  switch (this->pd_ev->et)
    {
    case EV_short:
      return this->pd_ev->u.sval == vc->ev ()->u.sval ? true : false;
    case EV_ushort:
      return this->pd_ev->u.usval == vc->ev ()->u.usval ? true : false;
    case EV_long:
      return this->pd_ev->u.lval == vc->ev ()->u.lval ? true : false;
    case EV_ulong:
      return this->pd_ev->u.ulval == vc->ev ()->u.ulval ? true : false;
    case EV_float:
      return this->pd_ev->u.fval == vc->ev ()->u.fval ? true : false;
    case EV_double:
      return this->pd_ev->u.dval == vc->ev ()->u.dval ? true : false;
    case EV_char:
      return this->pd_ev->u.cval == vc->ev ()->u.cval ? true : false;
    case EV_wchar:
      return this->pd_ev->u.wcval == vc->ev ()->u.wcval ? true : false;
    case EV_octet:
      return this->pd_ev->u.oval == vc->ev ()->u.oval ? true : false;
    case EV_bool:
      return this->pd_ev->u.lval == vc->ev ()->u.lval ? true : false;
    case EV_string:
      if (this->pd_ev->u.strval == 0)
        {
          if (vc->ev ()->u.strval == 0)
            {
                    return true;
            }
          else
            {
                    return false;
            }
        }
      else if (vc->ev ()->u.strval == 0)
        {
          return false;
        }
      else
        {
          return this->pd_ev->u.strval == vc->ev ()->u.strval ? true : false;
        }

    case EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      return this->pd_ev->u.llval == vc->ev ()->u.llval ? true : false;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return false;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
      return this->pd_ev->u.ullval == vc->ev ()->u.ullval ? true : false;
#else /* ! defined (ACE_LACKS_LONGLONG_T) */
      return false;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    case EV_longdouble:
    case EV_wstring:
    case EV_enum:
    case EV_void:
    case EV_none:
    case EV_any:
    case EV_object:
      return false;
    }

  return false;
}

AST_Decl *
AST_Expression::get_tdef (void) const
{
  return this->tdef;
}

AST_Param_Holder *
AST_Expression::param_holder (void) const
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
  if (n1 != 0)
    {
      n1->dump (o);
    }

  o << " " << s << " ";

  if (n2 != 0)
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
dump_expr_val (ACE_OSTREAM_TYPE &o,
               AST_Expression::AST_ExprValue *ev)
{
  switch (ev->et)
    {
    case AST_Expression::EV_short:
      o << ev->u.sval;
      break;
    case AST_Expression::EV_ushort:
      o << ev->u.usval;
      break;
    case AST_Expression::EV_long:
      o << ev->u.lval;
      break;
    case AST_Expression::EV_ulong:
      o << ev->u.ulval;
      break;
    case AST_Expression::EV_float:
      o << ev->u.fval;
      break;
    case AST_Expression::EV_double:
      o << ev->u.dval;
      break;
    case AST_Expression::EV_char:
      o << ev->u.cval;
      break;
    case AST_Expression::EV_wchar:
      o << ev->u.wcval;
      break;
    case AST_Expression::EV_octet:
      o << static_cast<int> (ev->u.oval);
      break;
    case AST_Expression::EV_bool:
      o << (ev->u.bval == true ? "TRUE" : "FALSE");
      break;
    case AST_Expression::EV_string:
      if (ev->u.strval != 0)
        ev->u.strval->dump(o);
    case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
//      o << ev->u.llval;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
    break;
  case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
//      o << ev->u.ullval;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
      break;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_enum:
    case AST_Expression::EV_none:
    case AST_Expression::EV_void:
    case AST_Expression::EV_any:
    case AST_Expression::EV_object:
      break;
    }
}

// Dump an AST_Expression node to the ostream o.
void
AST_Expression::dump (ACE_OSTREAM_TYPE &o)
{
  // See if it was a constant or was evaluated already.
  if (this->pd_ev != 0)
    {
      dump_expr_val (o,
                     this->pd_ev);
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
AST_Expression::destroy (void)
{
  if (0 != this->pd_ev && EV_string == this->pd_ev->et)
    {
      this->pd_ev->u.strval->destroy ();
      delete this->pd_ev->u.strval;
      this->pd_ev->u.strval = 0;
    }

  delete this->pd_ev;
  this->pd_ev = 0;

  if (this->pd_v1 != 0)
    {
      this->pd_v1->destroy ();
    }

  if (this->pd_v2 != 0)
    {
      this->pd_v2->destroy ();
    }

  delete this->pd_v1;
  this->pd_v1 = 0;

  delete this->pd_v2;
  this->pd_v2 = 0;

  if (this->pd_n != 0)
    {
      this->pd_n->destroy ();
    }

  delete this->pd_n;
  this->pd_n = 0;

  if (this->param_holder_ != 0)
    {
      this->param_holder_->destroy ();
      delete this->param_holder_;
      this->param_holder_ = 0;
    }
}

// Data accessors.

UTL_Scope *
AST_Expression::defined_in (void)
{
  return this->pd_defined_in;
}

void
AST_Expression::set_defined_in (UTL_Scope *d)
{
  this->pd_defined_in = d;
}

long
AST_Expression::line (void)
{
  return this->pd_line;
}

void
AST_Expression::set_line (long l)
{
  this->pd_line = l;
}

UTL_String *
AST_Expression::file_name (void)
{
  return this->pd_file_name;
}

void
AST_Expression::set_file_name (UTL_String *f)
{
  this->pd_file_name = f;
}

AST_Expression::ExprComb
AST_Expression::ec (void)
{
  return this->pd_ec;
}

AST_Expression::AST_ExprValue   *
AST_Expression::ev (void)
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
AST_Expression::v1 (void)
{
  return this->pd_v1;
}

void
AST_Expression::set_v1 (AST_Expression *e)
{
  this->pd_v1 = e;
}

AST_Expression *
AST_Expression::v2 (void)
{
  return this->pd_v2;
}

void
AST_Expression::set_v2 (AST_Expression *e)
{
  this->pd_v2 = e;
}

UTL_ScopedName *
AST_Expression::n (void)
{
  return this->pd_n;
}

void
AST_Expression::set_n (UTL_ScopedName *new_n)
{
  this->pd_n = new_n;
}
