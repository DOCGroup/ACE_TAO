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

#ifndef _AST_EXPRESSION_AST_EXPRESSION_HH
#define _AST_EXPRESSION_AST_EXPRESSION_HH

#pragma ident "%@(#)AST_Expression.h	1.39% %92/06/10% Sun Microsystems"

// Representation of expression values

/*
** DEPENDENCIES: NONE
**
** USE: Included from ast.hh
*/

// An expression (and maybe its value, if computed)
class	AST_Expression {
public:
  // Enum to define all the different operators to combine expressions
  enum ExprComb {
      EC_add		// '+'
    , EC_minus		// '-'
    , EC_mul		// '*'
    , EC_div		// '/'
    , EC_mod		// '%'
    , EC_or		// '|'
    , EC_xor		// '^'
    , EC_and		// '&'
    , EC_left		// '<<'
    , EC_right		// '>>'
    , EC_u_plus		// unary '+'
    , EC_u_minus	// unary '-'
    , EC_bit_neg	// '~'
    , EC_none		// No operator (missing)
    , EC_symbol		// a symbol (function or constant name)
  };
  
  // Enum to define the different kinds of evaluation possible
  // Extend this for more kinds of evaluation as required
  enum EvalKind {
      EK_const			// Must evaluate to constant
    , EK_positive_int		// Must evaluate to positive integer
  };

  // Enum to define expression type
  enum ExprType {
      EV_short			// Expression value is short
    , EV_ushort			// Expression value is unsigned short
    , EV_long			// Expression value is long
    , EV_ulong			// Expression value is unsigned long
    , EV_longlong		// Expression value is long long
    , EV_ulonglong		// Expression value is unsigned long long
    , EV_float			// Expression value is 32-bit float
    , EV_double			// Expression value is 64-bit float
    , EV_longdouble		// Expression value is 128-bit float
    , EV_char			// Expression value is char
    , EV_wchar			// Expression value is wide char	
    , EV_octet			// Expression value is unsigned char
    , EV_bool			// Expression value is boolean
    , EV_string			// Expression value is char *
    , EV_wstring		// Expression value is wchar_t *
    , EV_any			// Expression value is any of above
    , EV_void			// Expression value is void (absent)
    , EV_none			// Expression value is missing
  };

  // Structure to descrive value of constant expression and its type
  struct AST_ExprValue {
    union {
      short		sval;	// Contains short expression value
      unsigned short	usval;	// Contains unsigned short expr value
      long		lval;	// Contains long expression value
      unsigned long	ulval;	// Contains unsigned long expr value
      unsigned long	bval;	// Contains boolean expression value
      float		fval;	// Contains 32-bit float expr value
      double		dval;	// Contains 64-bit float expr value
      char		cval;	// Contains char expression value
      unsigned char	oval;	// Contains unsigned char expr value
      String	        *strval; // Contains String * expr value
      unsigned long	eval;	// Contains enumeration value
    } u;
    ExprType et;
  };

  // Operations

  // Constructor(s)
  AST_Expression(AST_Expression *v, ExprType t);

  AST_Expression(ExprComb c, AST_Expression *v1, AST_Expression *v2);

  AST_Expression(short		s);
  AST_Expression(unsigned short	us);
  AST_Expression(long		l);
  AST_Expression(long		l, ExprType t);
  AST_Expression(unsigned long	ul);
  AST_Expression(float		f);
  AST_Expression(double		d);
  AST_Expression(char		c);
  AST_Expression(unsigned char	uc);
  AST_Expression(String		*s);
  AST_Expression(UTL_ScopedName	*n);
  virtual ~AST_Expression () {}

  // Data Accessors
  UTL_Scope *defined_in();
  void set_defined_in(UTL_Scope *d);
  long line();
  void set_line(long l);
  String *file_name();
  void set_file_name(String *f);
  ExprComb ec();
  void set_ec(ExprComb new_ec);
  AST_ExprValue	*ev();
  void set_ev(AST_ExprValue *new_ev);
  AST_Expression *v1();
  void set_v1(AST_Expression *e);
  AST_Expression *v2();
  void set_v2(AST_Expression *e);
  UTL_ScopedName *n();
  void set_n(UTL_ScopedName *new_n);

  // AST Dumping
  virtual void			dump(ostream &o);

  // Other operations

  // Evaluation and value coercion
  virtual AST_ExprValue		*eval(EvalKind ek);
  virtual AST_ExprValue		*coerce(ExprType t);

  // Evaluate then store value inside this AST_Expression
  virtual void			evaluate(EvalKind ek);

  // Compare to AST_Expressions
  virtual long			operator==(AST_Expression *vc);
  virtual long			compare(AST_Expression *vc);

protected:
  // Evaluate different sets of operators
  virtual AST_ExprValue	*eval_bin_op(EvalKind ek);
  virtual AST_ExprValue	*eval_bit_op(EvalKind ek);
  virtual AST_ExprValue	*eval_un_op(EvalKind ek);
  virtual AST_ExprValue	*eval_symbol(EvalKind ek);

private:
  // Data
  UTL_Scope			*pd_defined_in;	// Scope
  long				pd_line;	// Line defined in
  String			*pd_file_name;	// What file defined in

  ExprComb			pd_ec;		// What combinator
  AST_ExprValue			*pd_ev;		// computed value
  AST_Expression		*pd_v1;		// 1st sub-expression
  AST_Expression		*pd_v2;		// 2nd sub-expression
  UTL_ScopedName		*pd_n;		// symbolic name (if any)

  // Operations

  // Fill out the lineno, filename and definition scope details
  void				fill_definition_details();

  // Internal evaluation
  virtual AST_ExprValue *eval_internal(EvalKind ek);
};

#endif           // _AST_EXPR_VAL_AST_EXPR_VAL_HH


