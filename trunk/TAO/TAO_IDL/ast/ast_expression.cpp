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

/* * ast_expression.cc - Implementation of class AST_Expression
 *
 * AST_Expression nodes denote IDL expressions used in the IDL input.
 */

#include	"idl.h"
#include	"idl_extern.h"

#include	<limits.h>
#include	<float.h>

ACE_RCSID(ast, ast_expression, "$Id$")

#undef	MAXCHAR
#define MAXCHAR	128

// Helper function to fill out the details of where this expression
// is defined
void
AST_Expression::fill_definition_details()
{
  pd_defined_in = idl_global->scopes()->depth() > 0
    ? idl_global->scopes()->top()
    : 0 ;
  pd_line       = idl_global->lineno();
  pd_file_name	= idl_global->filename();
}

/*
 * Constructor(s) and destructor
 */

/*
 * An AST_Expression denoting a symbolic name
 */
AST_Expression::AST_Expression(UTL_ScopedName *nm)
	      : pd_ec(EC_symbol),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(nm)
{
  fill_definition_details();
}

/*
 * An AST_Expression denoting a type coercion from another AST_Expression
 */
AST_Expression::AST_Expression(AST_Expression *v, ExprType t)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = v->coerce(t);
  if (pd_ev == NULL)
    idl_global->err()->coercion_error(v, t);
}

/*
 * An AST_Expression denoting a binary expression combination from
 * two other AST_Expressions
 */
AST_Expression::AST_Expression(ExprComb c, AST_Expression *ev1,
			       AST_Expression *ev2)
	      : pd_ec(c),
		pd_ev(NULL),
		pd_v1(ev1),
		pd_v2(ev2),
		pd_n(NULL)
{
  fill_definition_details();
}

/*
 * An AST_Expression denoting a short integer
 */
AST_Expression::AST_Expression(short sv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_short;
  pd_ev->u.sval = sv;
}

/*
 * An AST_Expression denoting an unsigned short integer
 */
AST_Expression::AST_Expression(unsigned short usv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_ushort;
  pd_ev->u.usval = usv;
}

/*
 * An AST_Expression denoting a long integer
 */
AST_Expression::AST_Expression(long lv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_long;
  pd_ev->u.lval = lv;
}

/*
 * An AST_Expression denoting a long integer being used as a boolean
 */
AST_Expression::AST_Expression(long lv, ExprType t)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = t;
  pd_ev->u.lval = lv;
}

/*
 * An AST_Expression denoting an unsigned long integer
 */
AST_Expression::AST_Expression(unsigned long ulv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_ulong;
  pd_ev->u.ulval = ulv;
}

/*
 * An AST_Expression denoting a 32-bit floating point number
 */
AST_Expression::AST_Expression(float fv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_float;
  pd_ev->u.fval = fv;
}

/*
 * An AST_Expression denoting a 64-bit floating point number
 */
AST_Expression::AST_Expression(double dv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_double;
  pd_ev->u.dval = dv;
}

/*
 * An AST_Expression denoting a character
 */
AST_Expression::AST_Expression(char cv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_char;
  pd_ev->u.cval = cv;
}

AST_Expression::AST_Expression(ACE_CDR::WChar wcv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_wchar;
  pd_ev->u.wcval = wcv;
}

/*
 * An AST_Expression denoting an octet (unsigned char)
 */
AST_Expression::AST_Expression(unsigned char ov)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_octet;
  pd_ev->u.oval = ov;
}

/*
 * An AST_Expression denotign a string (char * encapsulated as a String)
 */
AST_Expression::AST_Expression(String *sv)
	      : pd_ec(EC_none),
		pd_ev(NULL),
		pd_v1(NULL),
		pd_v2(NULL),
		pd_n(NULL)
{
  fill_definition_details();

  pd_ev = new AST_ExprValue;
  pd_ev->et = EV_string;
  pd_ev->u.strval = sv;
}

/*
 * Static operations
 */

/*
 * Perform the coercion from the given AST_ExprValue to the requested
 * ExprType. Return an AST_ExprValue if successful, NULL if failed.
 */
static AST_Expression::AST_ExprValue *
coerce_value(AST_Expression::AST_ExprValue *ev, AST_Expression::ExprType t)
{
  if (ev == NULL)
    return NULL;

  switch (t) {
  case AST_Expression::EV_short:
    switch (ev->et) {
    case AST_Expression::EV_short:
      return ev;
    case AST_Expression::EV_ushort:
      if (ev->u.usval > (unsigned short) SHRT_MAX)
	      return NULL;
      ev->u.sval = (short) ev->u.usval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval > (long) SHRT_MAX || ev->u.lval < (long) -(SHRT_MAX))
	      return NULL;
      ev->u.sval = (short) ev->u.lval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_ulong:
      if (ev->u.ulval > (unsigned long) SHRT_MAX)
	      return NULL;
      ev->u.sval = (short) ev->u.ulval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) SHRT_MAX || ev->u.llval < (ACE_CDR::LongLong) -(SHRT_MAX))
	      return NULL;
      ev->u.sval = (short) ev->u.llval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) SHRT_MAX)
	      return NULL;
      ev->u.sval = (short) ev->u.ullval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.sval = (short) ev->u.bval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval > (float) SHRT_MAX || ev->u.fval < (float) -(SHRT_MAX))
	      return NULL;
      ev->u.sval = (short) ev->u.fval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > (double) SHRT_MAX || ev->u.dval < (double) -(SHRT_MAX))
	      return NULL;
      ev->u.sval = (short) ev->u.dval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_char:
      ev->u.sval = (short) ev->u.cval;
      ev->et = AST_Expression::EV_short;
      return ev;
    case AST_Expression::EV_wchar:
      if (ev->u.wcval > (ACE_CDR::WChar) SHRT_MAX)
        return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_ushort:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval < 0)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.sval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_ushort:
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval > (long) USHRT_MAX || ev->u.lval < 0)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.lval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_ulong:
      if (ev->u.ulval > (unsigned long) USHRT_MAX)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.ulval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) USHRT_MAX || ev->u.llval < 0)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.llval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) USHRT_MAX)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.ullval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.usval = (unsigned short) ev->u.bval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval < 0.0 || ev->u.fval > (float) USHRT_MAX)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.fval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval < 0.0 || ev->u.dval > (double) USHRT_MAX)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.dval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_char:
      if ((signed char) ev->u.cval < 0)
	      return NULL;
      ev->u.usval = (unsigned short) ev->u.cval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_wchar:
      if (ev->u.wcval > (ACE_CDR::WChar) USHRT_MAX)
        return NULL;
      ev->u.usval = (short) ev->u.wcval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_octet:
      ev->u.usval = (unsigned short) ev->u.oval;
      ev->et = AST_Expression::EV_ushort;
      return ev;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_string:
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_long:
    switch (ev->et) {
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
      if (ev->u.ulval > (unsigned long) LONG_MAX)
	      return NULL;
      ev->u.lval = (long) ev->u.ulval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) LONG_MAX || ev->u.llval < (ACE_CDR::LongLong) -(LONG_MAX))
	      return NULL;
      ev->u.lval = (long) ev->u.llval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) LONG_MAX)
	      return NULL;
      ev->u.lval = (long) ev->u.ullval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.lval = (long) ev->u.bval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval > (float) LONG_MAX || ev->u.fval < (float) -(LONG_MAX))
	      return NULL;
      ev->u.lval = (long) ev->u.fval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > (double) LONG_MAX || ev->u.dval < (double) -(LONG_MAX))
	      return NULL;
      ev->u.lval = (long) ev->u.dval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_char:
      ev->u.lval = (long) ev->u.cval;
      ev->et = AST_Expression::EV_long;
      return ev;
    case AST_Expression::EV_wchar:
      if (ev->u.wcval > (ACE_CDR::WChar) LONG_MAX)
        return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_ulong:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval < 0)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.sval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_ushort:
      ev->u.ulval = (unsigned long) ev->u.usval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval < 0)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.lval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_ulong:
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) ULONG_MAX || ev->u.llval < 0)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.llval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) ULONG_MAX)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.ullval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.ulval = (unsigned long) ev->u.bval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval < 0.0 || ev->u.fval > (float) ULONG_MAX)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.fval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval < 0.0 || ev->u.dval > (double) ULONG_MAX)
	      return NULL;
      ev->u.ulval = (unsigned long) ev->u.dval;
      ev->et = AST_Expression::EV_ulong;
      return ev;
    case AST_Expression::EV_char:
      if ((signed char) ev->u.cval < 0)
	      return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_longlong:
    switch (ev->et) {
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
	      return NULL;
      ev->u.llval = (ACE_CDR::LongLong) ev->u.ullval;
      ev->et = AST_Expression::EV_longlong;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.llval = (ACE_CDR::LongLong) ev->u.bval;
      ev->et = AST_Expression::EV_longlong;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval > (float) ACE_INT64_MAX
          || ev->u.fval < (float) ACE_INT64_MIN)
	      return NULL;
      ev->u.llval = (ACE_CDR::LongLong) ev->u.fval;
      ev->et = AST_Expression::EV_longlong;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > (double) ACE_INT64_MAX
          || ev->u.dval < (double) ACE_INT64_MIN)
	      return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_ulonglong:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval < 0)
	      return NULL;
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.sval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_ushort:
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.usval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval < 0)
	      return NULL;
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.lval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_ulong:
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.ulval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval < 0)
	      return NULL;
      ev->u.ullval = (ACE_CDR::LongLong) ev->u.llval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_ulonglong:
      return ev;
    case AST_Expression::EV_bool:
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.bval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval < 0.0 || ev->u.fval > (float) ACE_UINT64_MAX)
	      return NULL;
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.fval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval < 0.0 || ev->u.dval > (double) ACE_UINT64_MAX)
	      return NULL;
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.dval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_char:
      if ((signed char) ev->u.cval < 0)
	      return NULL;
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.cval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_wchar:
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.wcval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_octet:
      ev->u.ullval = (ACE_CDR::ULongLong) ev->u.oval;
      ev->et = AST_Expression::EV_ulonglong;
      return ev;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_string:
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_bool:
    switch (ev->et) {
    case AST_Expression::EV_short:
      ev->u.bval = (ev->u.sval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_ushort:
      ev->u.bval = (ev->u.usval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_long:
      ev->u.bval = (ev->u.lval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_ulong:
      ev->u.bval = (ev->u.ulval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_longlong:
      ev->u.bval = (ev->u.llval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_ulonglong:
      ev->u.bval = (ev->u.ullval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_bool:
      return ev;
    case AST_Expression::EV_float:
      ev->u.bval = (ev->u.fval == 0.0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_double:
      ev->u.bval = (ev->u.dval == 0.0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_char:
      ev->u.bval = (ev->u.cval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_wchar:
      ev->u.bval = (ev->u.wcval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_octet:
      ev->u.bval = (ev->u.oval == 0) ? I_FALSE : I_TRUE;
      ev->et = AST_Expression::EV_bool;
      return ev;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_string:
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_float:
    switch (ev->et) {
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
      if (ev->u.llval > FLT_MAX || ev->u.llval < -(FLT_MAX))
       return NULL;
      ev->u.fval = (float) ev->u.llval;
      ev->et = AST_Expression::EV_float;
      return ev;
    case AST_Expression::EV_ulonglong:
      // Some compilers don't implement unsigned 64-bit to float conversions,
      // so we are stuck with the signed 64-bit max value.
      if (ev->u.ullval > ACE_UINT64_MAX
          || (ACE_CDR::LongLong) ev->u.ullval > ACE_FLT_MAX)
       return NULL;
      ev->u.fval = (float) ((ACE_CDR::LongLong) ev->u.ullval);
      ev->et = AST_Expression::EV_float;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.fval = (float) ((ev->u.bval == I_TRUE) ? 1.0 : 0.0);
      ev->et = AST_Expression::EV_float;
      return ev;
    case AST_Expression::EV_float:
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > FLT_MAX || ev->u.dval < -FLT_MAX)
	      return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_double:
    switch (ev->et) {
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
      ev->u.dval = (double) ev->u.llval;
      ev->et = AST_Expression::EV_double;
      return ev;
    case AST_Expression::EV_ulonglong:
      // Some compilers don't implement unsigned 64-bit to double
      // conversions, so we are stuck with the signed 64-bit max value.
      if (ev->u.ullval > ACE_INT64_MAX)
        return NULL;
      ev->u.dval = (double) ((ACE_CDR::LongLong) ev->u.ullval);
      ev->et = AST_Expression::EV_double;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.dval = (ev->u.bval == I_TRUE) ? 1.0 : 0.0;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_char:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval > (short) MAXCHAR || ev->u.sval < (short) -(MAXCHAR))
	      return NULL;
      ev->u.cval = (char) ev->u.sval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_ushort:
      if (ev->u.usval > (unsigned short) MAXCHAR)
	      return NULL;
      ev->u.cval = (char) ev->u.usval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval > (long) MAXCHAR || ev->u.lval < (long) -(MAXCHAR))
	      return NULL;
      ev->u.cval = (char) ev->u.lval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_ulong:
      if (ev->u.ulval > (unsigned long) MAXCHAR)
	      return NULL;
      ev->u.cval = (char) ev->u.ulval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) MAXCHAR || ev->u.llval < (ACE_CDR::LongLong) -(MAXCHAR))
	      return NULL;
      ev->u.cval = (char) ev->u.llval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) MAXCHAR)
	      return NULL;
      ev->u.cval = (char) ev->u.ullval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.cval = (char) ev->u.bval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval > (float) MAXCHAR || ev->u.fval < (float) -(MAXCHAR))
	      return NULL;
      ev->u.cval = (char) ev->u.fval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > (double) MAXCHAR || ev->u.dval < (double) -(MAXCHAR))
	      return NULL;
      ev->u.cval = (char) ev->u.dval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_char:
      return ev;
    case AST_Expression::EV_wchar:
      if (ev->u.wcval > (ACE_CDR::WChar) MAXCHAR)
        return NULL;
      ev->u.cval = (char) ev->u.wcval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_octet:
      if (ev->u.oval > (unsigned char) MAXCHAR)
	      return NULL;
      ev->u.cval = (char) ev->u.oval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_string:
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_wchar:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval < 0)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.sval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_ushort:
      ev->u.wcval = (ACE_CDR::WChar) ev->u.usval;
      ev->et = AST_Expression::EV_char;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval < 0)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.lval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_ulong:
      ev->u.wcval = (ACE_CDR::WChar) ev->u.ulval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval > (ACE_CDR::LongLong) ULONG_MAX || ev->u.llval < 0)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.llval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) ULONG_MAX)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.ullval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.wcval = (ACE_CDR::WChar) ev->u.bval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval > (float) ULONG_MAX || ev->u.fval < 0)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.fval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval > (double) ULONG_MAX || ev->u.dval < 0)
	      return NULL;
      ev->u.wcval = (ACE_CDR::WChar) ev->u.dval;
      ev->et = AST_Expression::EV_wchar;
      return ev;
    case AST_Expression::EV_char:
      if ((signed char) ev->u.cval < 0)
        return NULL;
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
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_octet:
    switch (ev->et) {
    case AST_Expression::EV_short:
      if (ev->u.sval < 0 || ev->u.sval > (short) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.sval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_ushort:
      if (ev->u.usval > (unsigned short) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.usval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_long:
      if (ev->u.lval < 0 || ev->u.lval > (long) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.lval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_ulong:
      if (ev->u.ulval > (unsigned long) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.ulval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_longlong:
      if (ev->u.llval < 0 || ev->u.llval > (ACE_CDR::LongLong) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.llval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_ulonglong:
      if (ev->u.ullval > (ACE_CDR::ULongLong) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.ullval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_bool:
      ev->u.oval = (ev->u.bval == I_FALSE) ? 1 : 0;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_float:
      if (ev->u.fval < 0.0 || ev->u.fval > (float) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.fval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_double:
      if (ev->u.dval < 0.0 || ev->u.dval > (double) (MAXCHAR << 1))
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.dval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_char:
      if ((signed char) ev->u.cval < 0)
	      return NULL;
      ev->u.oval = (unsigned char) ev->u.cval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_wchar:
      if (ev->u.wcval > (ACE_CDR::WChar) (MAXCHAR << 1))
        return NULL;
      ev->u.oval = (unsigned char) ev->u.wcval;
      ev->et = AST_Expression::EV_octet;
      return ev;
    case AST_Expression::EV_octet:
      return ev;
    case AST_Expression::EV_longdouble:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_string:
    case AST_Expression::EV_any:
    case AST_Expression::EV_void:
    case AST_Expression::EV_none:
      return NULL;
    }
  case AST_Expression::EV_any:
    switch (ev->et) {
    case AST_Expression::EV_any:
      return ev;
    default:
      return NULL;
    }
  case AST_Expression::EV_void:
    switch (ev->et) {
    case AST_Expression::EV_void:
      return ev;
    default:
      return NULL;
    }
  case AST_Expression::EV_none:
    return NULL;
  case AST_Expression::EV_string:
    switch (ev->et) {
    case AST_Expression::EV_string:
      return ev;
    default:
      return NULL;
    }
  case AST_Expression::EV_longdouble:
  case AST_Expression::EV_wstring:
    return NULL;
  }
  return NULL;
}

/*
 * Evaluate the expression wrt the evaluation kind requested. Supported
 * evaluation kinds are
 * - EK_const:		The expression must evaluate to a constant
 * - EK_positive_int:	The expression must further evaluate to a
 *			positive integer
 */
static AST_Expression::AST_ExprValue *
eval_kind(AST_Expression::AST_ExprValue *ev, AST_Expression::EvalKind ek)
{
  if (ek == AST_Expression::EK_const)
    return ev;
  if (ek == AST_Expression::EK_positive_int)
    return coerce_value(ev, AST_Expression::EV_ulong);

  return NULL;
}

/*
 * Private operations
 */

/*
 * Apply binary operators to an AST_Expression after evaluating
 * its sub-expressions.
 * Operations supported:
 *
 * '+', '-', '*', '/'
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval_bin_op(AST_Expression::EvalKind ek)
{
  AST_ExprValue	*retval = NULL;

  if (ek != EK_const && ek != EK_positive_int)
    return NULL;
  if (pd_v1 == NULL || pd_v2 == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->eval_internal(ek));
  if (pd_v1->ev() == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->coerce(EV_double));
  if (pd_v1->ev() == NULL)
    return NULL;
  pd_v2->set_ev(pd_v2->eval_internal(ek));
  if (pd_v2->ev() == NULL)
    return NULL;
  pd_v2->set_ev(pd_v2->coerce(EV_double));
  if (pd_v2->ev() == NULL)
    return NULL;

  retval = new AST_ExprValue;
  retval->et = EV_double;

  switch (pd_ec) {
  case EC_mod:
    if (pd_v2->ev()->u.lval == 0)
      return NULL;
    retval->u.lval = pd_v1->ev()->u.lval % pd_v2->ev()->u.lval;
    break;
  case EC_add:
    retval->u.dval = pd_v1->ev()->u.dval + pd_v2->ev()->u.dval;
    break;
  case EC_minus:
    retval->u.dval = pd_v1->ev()->u.dval - pd_v2->ev()->u.dval;
    break;
  case EC_mul:
    retval->u.dval = pd_v1->ev()->u.dval * pd_v2->ev()->u.dval;
    break;
  case EC_div:
    if (pd_v2->ev()->u.dval == 0.0)
      return NULL;
    retval->u.dval = pd_v1->ev()->u.dval / pd_v2->ev()->u.dval;
    break;
  default:
    return NULL;
  }
  return retval;
}

/*
 * Apply bitwise operations to an AST_Expression after evaluating
 * its sub-expressions.
 * Operations supported:
 *
 * '%', '|', '&', '^', '<<', '>>'
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval_bit_op(AST_Expression::EvalKind ek)
{
  AST_Expression::AST_ExprValue	*retval=NULL;

  if (ek != EK_const && ek != EK_positive_int)
    return NULL;
  if (pd_v1 == NULL || pd_v2 == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->eval_internal(ek));
  if (pd_v1->ev() == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->coerce(EV_long));
  if (pd_v1->ev() == NULL)
    return NULL;
  pd_v2->set_ev(pd_v2->eval_internal(ek));
  if (pd_v2->ev() == NULL)
    return NULL;
  pd_v2->set_ev(pd_v2->coerce(EV_long));
  if (pd_v2->ev() == NULL)
    return NULL;

  retval = new AST_ExprValue;
  retval->et = EV_long;

  switch (pd_ec) {
  case EC_or:
    retval->u.lval = pd_v1->ev()->u.lval | pd_v2->ev()->u.lval;
    break;
  case EC_xor:
    retval->u.lval = pd_v1->ev()->u.lval ^ pd_v2->ev()->u.lval;
    break;
  case EC_and:
    retval->u.lval = pd_v1->ev()->u.lval & pd_v2->ev()->u.lval;
    break;
  case EC_left:
    retval->u.lval = pd_v1->ev()->u.lval << pd_v2->ev()->u.lval;
    break;
  case EC_right:
    retval->u.lval = pd_v1->ev()->u.lval >> pd_v2->ev()->u.lval;
    break;
  default:
    return NULL;
  }
  return retval;
}

/*
 * Apply unary operators to an AST_Expression after evaluating its
 * sub-expression.
 * Operations supported:
 *
 * '-', '+', '~'
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval_un_op(AST_Expression::EvalKind ek)
{
  AST_ExprValue	*retval = NULL;

  if (pd_ev != NULL)
    return pd_ev;

  if (ek != EK_const && ek != EK_positive_int)
    return NULL;
  if (pd_v1 == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->eval_internal(ek));
  if (pd_v1->ev() == NULL)
    return NULL;
  pd_v1->set_ev(pd_v1->coerce(EV_double));
  if (pd_v1->ev() == NULL)
    return NULL;

  retval = new AST_ExprValue;
  retval->et = EV_double;

  switch (pd_ec) {
  case EC_u_plus:
    retval->u.lval = pd_v1->ev()->u.lval;
    break;
  case EC_u_minus:
    retval->u.lval = -(pd_v1->ev()->u.lval);
    break;
  case EC_bit_neg:
    pd_v1->set_ev(pd_v1->coerce(EV_long));
    if (pd_v1->ev() == NULL)
      return NULL;
    retval->u.lval = ~pd_v1->ev()->u.lval;
    break;
  default:
    return NULL;
  }
  return retval;
}

/*
 * Evaluate a symbolic AST_Expression by looking up the named
 * symbol.
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval_symbol(AST_Expression::EvalKind ek)
{
  UTL_Scope		*s = 0;
  AST_Decl		*d = 0;
  AST_Constant		*c = 0;

  /*
   * Is there a symbol stored?
   */
  if (pd_n == NULL) {
    idl_global->err()->eval_error(this);
    return NULL;
  }
  /*
   * Get current scope for lookup
   */
  if (idl_global->scopes()->depth() > 0)
     s = idl_global->scopes()->top_non_null();
  if (s == NULL) {
    idl_global->err()->lookup_error(pd_n);
    return NULL;
  }
  /*
   * Do lookup
   */
  d = s->lookup_by_name(pd_n, I_TRUE);
  if (d == NULL) {
    idl_global->err()->lookup_error(pd_n);
    return NULL;
  }
  /*
   * Is it a constant?
   */
  if (d->node_type() != AST_Decl::NT_const &&
      d->node_type() != AST_Decl::NT_enum_val) {
    idl_global->err()->constant_expected(pd_n, d);
    return NULL;
  }
  /*
   * OK, now evaluate the constant we just got, to produce its value
   */
  c = AST_Constant::narrow_from_decl(d);
  if (c == NULL) return NULL;
  return c->constant_value()->eval_internal(ek);
}

/*
 * Coerce "this" to the ExprType required. Returns a copy of the
 * original ExprValue with the coercion applied, if successful, or
 * NULL if failed.
 */
AST_Expression::AST_ExprValue *
AST_Expression::coerce(AST_Expression::ExprType t)
{
  AST_ExprValue *copy;

  /*
   * Is it already of the right type?
   */
  if (pd_ev != NULL && pd_ev->et == t)
    return pd_ev;
  /*
   * OK, must coerce
   *
   * First, evaluate it, then try to coerce result type
   * If already evaluated, return the result
   */
  pd_ev = eval_internal(EK_const);
  if (pd_ev == NULL)
    return NULL;

  /*
   * Create a copy to contain coercion result
   */
  copy = new AST_ExprValue;

  copy->et = pd_ev->et;
  switch (pd_ev->et) {
  case EV_longlong:
  case EV_longdouble:
  case EV_wchar:
  case EV_wstring:
  case EV_void:
  case EV_none:
  case EV_any:
    return NULL;
  case EV_short:
    copy->u.sval = pd_ev->u.sval;
    break;
  case EV_ushort:
    copy->u.usval = pd_ev->u.usval;
    break;
  case EV_long:
    copy->u.lval = pd_ev->u.lval;
    break;
  case EV_ulong:
    copy->u.ulval = pd_ev->u.ulval;
    break;
  case EV_ulonglong:
    copy->u.ullval = pd_ev->u.ullval;
    break;
  case EV_bool:
    copy->u.bval = pd_ev->u.bval;
    break;
  case EV_float:
    copy->u.fval = pd_ev->u.fval;
    break;
  case EV_double:
    copy->u.dval = pd_ev->u.dval;
    break;
  case EV_char:
    copy->u.cval = pd_ev->u.cval;
    break;
  case EV_octet:
    copy->u.oval = pd_ev->u.oval;
    break;
  case EV_string:
    copy->u.strval = pd_ev->u.strval;
    break;
  }

  return coerce_value(copy, t);
}

/*
 * Eval used internally
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval_internal(AST_Expression::EvalKind ek)
{
  /*
   * Already evaluated?
   */
  if (pd_ev != NULL)
    return eval_kind(pd_ev, ek);
  /*
   * OK, must evaluate operator
   */
  switch (pd_ec) {
  case EC_add:
  case EC_minus:
  case EC_mul:
  case EC_div:
  case EC_mod:
    pd_ev = eval_bin_op(ek);
    return eval_kind(pd_ev, ek);
  case EC_or:
  case EC_xor:
  case EC_and:
  case EC_left:
  case EC_right:
    pd_ev = eval_bit_op(ek);
    return eval_kind(pd_ev, ek);
  case EC_u_plus:
  case EC_u_minus:
  case EC_bit_neg:
    pd_ev = eval_un_op(ek);
    return eval_kind(pd_ev, ek);
  case EC_symbol:
    pd_ev = eval_symbol(ek);
    return eval_kind(pd_ev, ek);
  case EC_none:
    return NULL;
  }
  return NULL;
}

/*
 * Public operations
 */

/*
 * Evaluate an AST_Expression, producing an AST_ExprValue or NULL
 */
AST_Expression::AST_ExprValue *
AST_Expression::eval(AST_Expression::EvalKind ek)
{
  AST_Expression::AST_ExprValue *v = NULL;

  /*
   * Call internal evaluator which does not coerce value to
   * EvalKind-expected format
   */
  v = eval_internal(ek);
  /*
   * Then coerce according to EvalKind-expected format
   */
  return eval_kind(v, ek);
}

/*
 * Evaluate "this", assigning the value to the pd_ev field.
 */
void
AST_Expression::evaluate(EvalKind ek)
{
  pd_ev = eval_internal(ek);
  pd_ev = eval_kind(pd_ev, ek);
}

/*
 * Expression equality comparison operator
 */
long
AST_Expression::operator==(AST_Expression *vc)
{
  if (pd_ec != vc->ec())
    return I_FALSE;
  evaluate(EK_const);
  vc->evaluate(EK_const);
  if (pd_ev == NULL || vc->ev() == NULL)
    return I_FALSE;
  if (pd_ev->et != vc->ev()->et)
    return I_FALSE;
  switch (pd_ev->et) {
  case EV_short:
    return (pd_ev->u.sval == vc->ev()->u.sval) ? I_TRUE : I_FALSE;
  case EV_ushort:
    return (pd_ev->u.usval == vc->ev()->u.usval) ? I_TRUE : I_FALSE;
  case EV_long:
    return (pd_ev->u.lval == vc->ev()->u.lval) ? I_TRUE : I_FALSE;
  case EV_ulong:
    return (pd_ev->u.ulval == vc->ev()->u.ulval) ? I_TRUE : I_FALSE;
  case EV_float:
    return (pd_ev->u.fval == vc->ev()->u.fval) ? I_TRUE : I_FALSE;
  case EV_double:
    return (pd_ev->u.dval == vc->ev()->u.dval) ? I_TRUE : I_FALSE;
  case EV_char:
    return (pd_ev->u.cval == vc->ev()->u.cval) ? I_TRUE : I_FALSE;
  case EV_octet:
    return (pd_ev->u.oval == vc->ev()->u.oval) ? I_TRUE : I_FALSE;
  case EV_bool:
    return (pd_ev->u.lval == vc->ev()->u.lval) ? I_TRUE : I_FALSE;
  case EV_string:
    if (pd_ev->u.strval == NULL) {
      if (vc->ev()->u.strval == NULL)
	return I_TRUE;
      else
	return I_FALSE;
    } else if (vc->ev()->u.strval == NULL)
      return I_FALSE;
    else
      return (pd_ev->u.strval == vc->ev()->u.strval) ? I_TRUE : I_FALSE;
  case EV_longlong:
  case EV_ulonglong:
  case EV_longdouble:
  case EV_wchar:
  case EV_wstring:
  case EV_any:
  case EV_void:
  case EV_none:
    return I_FALSE;
  }
  return I_FALSE;
}

long
AST_Expression::compare(AST_Expression *vc)
{
  if (pd_ec != vc->ec())
    return I_FALSE;
  evaluate(EK_const);
  vc->evaluate(EK_const);
  if (pd_ev == NULL || vc->ev() == NULL)
    return I_FALSE;
  if (pd_ev->et != vc->ev()->et)
    return I_FALSE;
  switch (pd_ev->et) {
  case EV_short:
    return (pd_ev->u.sval == vc->ev()->u.sval) ? I_TRUE : I_FALSE;
 case EV_ushort:
    return (pd_ev->u.usval == vc->ev()->u.usval) ? I_TRUE : I_FALSE;
  case EV_long:
    return (pd_ev->u.lval == vc->ev()->u.lval) ? I_TRUE : I_FALSE;
  case EV_ulong:
    return (pd_ev->u.ulval == vc->ev()->u.ulval) ? I_TRUE : I_FALSE;
  case EV_float:
    return (pd_ev->u.fval == vc->ev()->u.fval) ? I_TRUE : I_FALSE;
  case EV_double:
    return (pd_ev->u.dval == vc->ev()->u.dval) ? I_TRUE : I_FALSE;
  case EV_char:
    return (pd_ev->u.cval == vc->ev()->u.cval) ? I_TRUE : I_FALSE;
  case EV_octet:
    return (pd_ev->u.oval == vc->ev()->u.oval) ? I_TRUE : I_FALSE;
  case EV_bool:
    return (pd_ev->u.lval == vc->ev()->u.lval) ? I_TRUE : I_FALSE;
  case EV_string:
    if (pd_ev->u.strval == NULL) {
      if (vc->ev()->u.strval == NULL)
	return I_TRUE;
      else
	return I_FALSE;
    } else if (vc->ev()->u.strval == NULL)
      return I_FALSE;
    else
      return (pd_ev->u.strval == vc->ev()->u.strval) ? I_TRUE : I_FALSE;
  case EV_longlong:
  case EV_ulonglong:
  case EV_longdouble:
  case EV_wchar:
  case EV_wstring:
  case EV_any:
  case EV_void:
  case EV_none:
    return I_FALSE;
  }
  return I_FALSE;
}

/*
 * Redefinition of inherited virtual operations
 */

/*
 * Helper functions for expression dumpers
 */

/*
 * Dump this binary AST_Expression node to the ostream o
 */
static void
dump_binary_expr(ostream &o, char *s, AST_Expression *n1, AST_Expression *n2)
{
  if (n1 != NULL) n1->dump(o);
  o << " " << s << " ";
  if (n2 != NULL) n2->dump(o);
}

/*
 * Dump this unary AST_Expression node to the ostream o
 */
static void
dump_unary_expr(ostream &o, char *s, AST_Expression *e)
{
  o << s;
  e->dump(o);
}

/*
 * Dump the supplied AST_ExprValue to the ostream o
 */
static void
dump_expr_val(ostream &o, AST_Expression::AST_ExprValue *ev)
{
  switch (ev->et) {
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
  case AST_Expression::EV_octet:
    o << ev->u.oval;
    break;
  case AST_Expression::EV_bool:
    o << (ev->u.bval == I_TRUE) ? "TRUE" : "FALSE";
    break;
  case AST_Expression::EV_string:
    if (ev->u.strval != NULL)
      ev->u.strval->dump(o);
  case AST_Expression::EV_longlong:
  case AST_Expression::EV_ulonglong:
  case AST_Expression::EV_longdouble:
  case AST_Expression::EV_wchar:
  case AST_Expression::EV_wstring:
  case AST_Expression::EV_any:
  case AST_Expression::EV_none:
  case AST_Expression::EV_void:
    break;
  }
}

/*
 * Dump an AST_Expression node to the ostream o
 */
void
AST_Expression::dump(ostream &o)
{
  /*
   * See if it was a constant or was evaluated already
   */
  if (pd_ev != NULL) {
    dump_expr_val(o, pd_ev);
    return;
  }
  /*
   * OK, must print out an expression
   */
  switch (pd_ec) {
    /*
     * Binary expressions:
     */
  case EC_add:
    dump_binary_expr(o, "+", pd_v1, pd_v2);
    break;
  case EC_minus:
    dump_binary_expr(o, "-", pd_v1, pd_v2);
    break;
  case EC_mul:
    dump_binary_expr(o, "*", pd_v1, pd_v2);
    break;
  case EC_div:
    dump_binary_expr(o, "/", pd_v1, pd_v2);
    break;
  case EC_mod:
    dump_binary_expr(o, "%", pd_v1, pd_v2);
    break;
  case EC_or:
    dump_binary_expr(o, "|", pd_v1, pd_v2);
    break;
  case EC_xor:
    dump_binary_expr(o, "^", pd_v1, pd_v2);
    break;
  case EC_and:
    dump_binary_expr(o, "&", pd_v1, pd_v2);
    break;
  case EC_left:
    dump_binary_expr(o, "<<", pd_v1, pd_v2);
    break;
  case EC_right:
    dump_binary_expr(o, ">>", pd_v1, pd_v2);
    break;
    /*
     * Unary expressions:
     */
  case EC_u_plus:
    dump_unary_expr(o, "+", pd_v1);
    break;
  case EC_u_minus:
    dump_unary_expr(o, "-", pd_v1);
    break;
  case EC_bit_neg:
    dump_unary_expr(o, "~", pd_v1);
    break;
    /*
     * Unevaluated symbol
     */
  case EC_symbol:
    pd_n->dump(o);
    break;
  case EC_none:
    break;
  default:
    o << GTDEVEL("unsupported dump mode for expression with ec == ")
      << (int) pd_ec ;
    break;
  }
}

/*
 * Data accessors
 */

UTL_Scope *
AST_Expression::defined_in()
{
  return pd_defined_in;
}

void
AST_Expression::set_defined_in(UTL_Scope *d)
{
  pd_defined_in = d;
}

long
AST_Expression::line()
{
  return pd_line;
}

void
AST_Expression::set_line(long l)
{
  pd_line = l;
}

String *
AST_Expression::file_name()
{
  return pd_file_name;
}

void
AST_Expression::set_file_name(String *f)
{
  pd_file_name = f;
}

AST_Expression::ExprComb
AST_Expression::ec()
{
  return pd_ec;
}

void
AST_Expression::set_ec(AST_Expression::ExprComb new_ec)
{
  pd_ec = new_ec;
}

AST_Expression::AST_ExprValue	*
AST_Expression::ev()
{
  return pd_ev;
}

void
AST_Expression::set_ev(AST_Expression::AST_ExprValue *new_ev)
{
  pd_ev = new_ev;
}

AST_Expression *
AST_Expression::v1()
{
  return pd_v1;
}

void
AST_Expression::set_v1(AST_Expression *e)
{

  pd_v1 = e;
}

AST_Expression *
AST_Expression::v2()
{
  return pd_v2;
}

void
AST_Expression::set_v2(AST_Expression *e)
{
  pd_v2 = e;
}

UTL_ScopedName *
AST_Expression::n()
{
  return pd_n;
}

void
AST_Expression::set_n(UTL_ScopedName *new_n)
{
  pd_n = new_n;
}
