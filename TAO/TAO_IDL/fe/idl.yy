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

/*
 * idl.yy - YACC grammar for IDL 1.1
 */

/* Declarations */

%{
#include "idl.h"
#include "idl_extern.h"

#include "fe_private.h"

#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern	"C" int yywrap();
#endif	// (defined(apollo) || defined(hpux)) && defined(__cplusplus)

%}

/*
 * Declare the type of values in the grammar
 */

%union {
  AST_Decl		*dcval;		/* Decl value		*/
  UTL_StrList		*slval;		/* String list		*/
  UTL_NameList		*nlval;		/* Name list		*/
  UTL_ExprList		*elval;		/* Expression list	*/
  UTL_LabelList		*llval;		/* Label list		*/
  UTL_DeclList		*dlval;		/* Declaration list	*/
  FE_InterfaceHeader	*ihval;		/* Interface header	*/
  AST_Expression	*exval;		/* Expression value	*/
  AST_UnionLabel	*ulval;		/* Union label		*/
  AST_Field		*ffval;		/* Field value		*/
  AST_Expression::ExprType etval;	/* Expression type	*/
  AST_Argument::Direction dival;	/* Argument direction	*/
  AST_Operation::Flags	ofval;		/* Operation flags	*/
  FE_Declarator		*deval;		/* Declarator value	*/
  idl_bool		bval;		/* Boolean value	*/
  long			ival;		/* Long value		*/
  double		dval;		/* Double value		*/
  float			fval;		/* Float value		*/
  char			cval;		/* Char value		*/
  
  String		*sval;		/* String value		*/
  char			*strval;	/* char * value		*/
  Identifier		*idval;		/* Identifier		*/
  UTL_IdList		*idlist;	/* Identifier list	*/
}

/*
 * Token types: These are returned by the lexer
 */

%token <strval>	IDENTIFIER

%token		CONST
%token		MODULE
%token		INTERFACE
%token		TYPEDEF
%token		LONG
%token		SHORT
%token		UNSIGNED
%token		DOUBLE
%token		FLOAT
%token		CHAR
%token		WCHAR
%token		OCTET
%token		BOOLEAN
%token		ANY
%token		STRUCT
%token		UNION
%token		SWITCH
%token		ENUM
%token		SEQUENCE
%token		STRING
%token		WSTRING
%token		EXCEPTION
%token		CASE
%token		DEFAULT
%token		READONLY
%token		ATTRIBUTE
%token		ONEWAY
%token		IDEMPOTENT
%token		VOID
%token		IN
%token		OUT
%token		INOUT
%token		RAISES
%token		CONTEXT

%token <ival>	INTEGER_LITERAL
%token <sval>	STRING_LITERAL
%token <cval>	CHARACTER_LITERAL
%token <dval>	FLOATING_PT_LITERAL
%token		TRUETOK
%token		FALSETOK

%token <strval>	SCOPE_DELIMITOR
%token		LEFT_SHIFT
%token		RIGHT_SHIFT

/*
 * These are production names:
 */

%type <dcval>	type_spec simple_type_spec constructed_type_spec
%type <dcval>	template_type_spec sequence_type_spec string_type_spec
%type <dcval>	struct_type enum_type switch_type_spec union_type
%type <dcval>	array_declarator op_type_spec seq_head wstring_type_spec
%type <dcval>   param_type_spec

%type <idlist>	scoped_name
%type <slval>	opt_context at_least_one_string_literal
%type <slval>	string_literals

%type <nlval>	at_least_one_scoped_name scoped_names inheritance_spec
%type <nlval>	opt_raises

%type <elval>	at_least_one_array_dim array_dims

%type <llval>	at_least_one_case_label case_labels

%type <dlval>	at_least_one_declarator declarators
%type <dlval>   at_least_one_simple_declarator simple_declarators

%type <ihval>	interface_header

%type <exval>	expression const_expr or_expr xor_expr and_expr shift_expr
%type <exval>	add_expr mult_expr unary_expr primary_expr literal
%type <exval>	positive_int_expr array_dim

%type <ulval>	case_label

%type <ffval>	element_spec

%type <etval>	const_type integer_type char_type boolean_type
%type <etval>	floating_pt_type any_type signed_int
%type <etval>	unsigned_int base_type_spec octet_type

%type <dival>	direction

%type <ofval>	opt_op_attribute

%type <deval>	declarator simple_declarator complex_declarator

%type <bval>	opt_readonly

%type <idval>	interface_decl id

%%

/*
 * Production starts here
 */
start :	definitions ;

definitions
	: definition definitions
	| /* empty */
	;

definition
	: type_dcl
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| const_dcl
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| exception
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| interface_def
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| module
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| error
	{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
	';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
	;

module	: MODULE
	  {
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
	  IDENTIFIER
	  {
	    UTL_ScopedName 	*n =
		new UTL_ScopedName(new Identifier($3, 1, 0, I_FALSE), NULL);
	    AST_Module		*m = NULL;
	    UTL_Scope		*s = idl_global->scopes()->top_non_null();
	    UTL_StrList		*p = idl_global->pragmas();	

	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleIDSeen);
	    /*
	     * Make a new module and add it to the enclosing scope
	     */
	    if (s != NULL) {
	      m = idl_global->gen()->create_module(n, p);
	      (void) s->fe_add_module(m);
	    }
	    /*
	     * Push it on the stack
	     */
	    idl_global->scopes()->push(m);
	  }
          '{'
	  {
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
	  definitions
	  {
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
	  '}'
	  {
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleQsSeen);
	    /*
	     * Finished with this module - pop it from the scope stack
	     */
	    idl_global->scopes()->pop();
	  }
	  ;

interface_def
	: interface
	| forward
	;

interface :
	interface_header
	{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  AST_Decl	*v = NULL;
	  UTL_StrList   *p = idl_global->pragmas();
	  AST_Decl	*d = NULL;
	  AST_Interface *fd = NULL;

	  /*
	   * Make a new interface node and add it to its enclosing scope
	   */
	  if (s != NULL && $1 != NULL) {
	    i = idl_global->gen()->create_interface($1->interface_name(),
						    $1->inherits(),
						    $1->n_inherits(),
						    p);
	    if (i != NULL &&
		(d = s->lookup_by_name(i->name(), I_FALSE)) != NULL) {
	      /*
	       * See if we're defining a forward declared interface.
	       */
	      if (d->node_type() == AST_Decl::NT_interface) {
		/*
		 * Narrow to an interface
		 */
		fd = AST_Interface::narrow_from_decl(d);
		/*
		 * Successful?
		 */
		if (fd == NULL) {
		  /*
		   * Should we give an error here?
		   */
		}
		/*
		 * If it is a forward declared interface..
		 */
		else if (!fd->is_defined()) {
		  /*
		   * Check if redefining in same scope
		   */
		  if (fd->defined_in() != s) {
		    idl_global->err()
		       ->error3(UTL_Error::EIDL_SCOPE_CONFLICT,
				i,
				fd,
				ScopeAsDecl(s));
		  }
		  /*
		   * All OK, do the redefinition
		   */
		  else {
		    fd->set_inherits($1->inherits());
		    fd->set_n_inherits($1->n_inherits());
		    /*
		     * Update place of definition
		     */
		    fd->set_imported(idl_global->imported());
		    fd->set_in_main_file(idl_global->in_main_file());
		    fd->set_line(idl_global->lineno());
		    fd->set_file_name(idl_global->filename());
		    fd->add_pragmas(p);
		    /*
		     * Use full definition node
		     */
		    delete i;
		    i = fd;
		  }
	        }
	      }
	    }
	    /*
	     * Add the interface to its definition scope
	     */
	    (void) s->fe_add_interface(i);
	  }
	  /*
	   * Push it on the scope stack
	   */
	  idl_global->scopes()->push(i);
        }
	'{'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
	exports
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
	'}'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  idl_global->scopes()->pop();
	}
	;

interface_decl:
	 INTERFACE
	 {
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
	 id
	 {
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   $$ = $3;
	 }
	;

interface_header :
	interface_decl inheritance_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  $$ = new FE_InterfaceHeader(new UTL_ScopedName($1, NULL), $2);
	}
	;

inheritance_spec
	: ':'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
	  at_least_one_scoped_name
	{
	  $$ = $3;
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

exports
	: exports export
	| /* EMPTY */
	;

export
	: type_dcl
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| const_dcl
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| exception
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| attribute
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| operation
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
	  ';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
	| error
	{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
	';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
	;

at_least_one_scoped_name :
	scoped_name scoped_names
	{
	  $$ = new UTL_NameList($1, $2);
	}
	;

scoped_names
	: scoped_names
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
	  scoped_name
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopedNameSeen);

	  if ($1 == NULL)
	    $$ = new UTL_NameList($4, NULL);
	  else {
	    $1->nconc(new UTL_NameList($4, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

scoped_name
	: id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  $$ = new UTL_IdList($1, NULL);
	}
	| SCOPE_DELIMITOR
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
	  id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  $$ = new UTL_IdList(new Identifier($1, 1, 0, I_FALSE),
			      new UTL_IdList($3, NULL));
	}
	| scoped_name
	  SCOPE_DELIMITOR
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
	  id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  $1->nconc(new UTL_IdList($4, NULL));
	  $$ = $1;
	}
	;

id: IDENTIFIER
        {
            $$ = new Identifier($1, 1, 0, I_FALSE);
        }
        ;

forward :
	interface_decl
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName($1, NULL);
	  AST_InterfaceFwd	*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_ForwardDeclSeen);
	  /*
	   * Create a node representing a forward declaration of an
	   * interface. Store it in the enclosing scope
	   */
	  if (s != NULL) {
	    f = idl_global->gen()->create_interface_fwd(n, p);
	    (void) s->fe_add_interface_fwd(f);
	  }
	}
	;

const_dcl :
	CONST
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
	const_type
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
	id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
	'='
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
	expression
	{
	  UTL_ScopedName	*n = new UTL_ScopedName($5, NULL);
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Constant		*c = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstExprSeen);
	  /*
	   * Create a node representing a constant declaration. Store
	   * it in the enclosing scope
	   */
	  if ($9 != NULL && s != NULL) {
	    if ($9->coerce($3) == NULL)
	      idl_global->err()->coercion_error($9, $3);
	    else {
	      c =
		idl_global->gen()->create_constant($3, $9, n, p);
	      (void) s->fe_add_constant(c);
	    }
	  }
	}
	;

const_type
	: integer_type
	| char_type
	| octet_type
	| boolean_type
	| floating_pt_type
	| string_type_spec
	{
	  $$ = AST_Expression::EV_string;
	}
	| wstring_type_spec
	{
	  $$ = AST_Expression::EV_wstring;
        }
	| scoped_name
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*c = NULL;
	  AST_Typedef		*t = NULL;

	  /*
	   * If the constant's type is a scoped name, it must resolve
	   * to a scalar constant type
	   */
	  if (s != NULL && (d = s->lookup_by_name($1, I_TRUE)) != NULL) {
	    /*
	     * Look through typedefs
	     */
	    while (d->node_type() == AST_Decl::NT_typedef) {
	      t = AST_Typedef::narrow_from_decl(d);
	      if (t == NULL)
	        break;
	      d = t->base_type();
	    }
	    if (d == NULL)
	      $$ = AST_Expression::EV_any;
	    else if (d->node_type() == AST_Decl::NT_pre_defined) {
	      c = AST_PredefinedType::narrow_from_decl(d);
	      if (c != NULL) {
	         $$ = idl_global->PredefinedTypeToExprType(c->pt());
	      } else {
	         $$ = AST_Expression::EV_any;
	      }
	    } else
	      $$ = AST_Expression::EV_any;
	  } else
	    $$ = AST_Expression::EV_any;
	}
	;

expression : const_expr	;

const_expr : or_expr ;

or_expr : xor_expr
	| or_expr '|' xor_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_or, $1, $3);
	}
	;

xor_expr
	: and_expr
	| xor_expr '^' and_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_xor, $1, $3);
	}
	;

and_expr
	: shift_expr
	| and_expr '&' shift_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_and, $1, $3);
	}
	;

shift_expr
	: add_expr
	| shift_expr LEFT_SHIFT add_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_right,$1,$3);
	}
	| shift_expr RIGHT_SHIFT add_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_left,$1,$3);
	}
	;

add_expr
	: mult_expr
	| add_expr '+' mult_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_add, $1, $3);
	}
	| add_expr '-' mult_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_minus,$1,$3);
	}
	;

mult_expr
	: unary_expr
	| mult_expr '*' unary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_mul, $1, $3);
	}
	| mult_expr '/' unary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_div, $1, $3);
	}
	| mult_expr '%' unary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_mod, $1, $3);
	}
	;

unary_expr
	: primary_expr
	| '+' primary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      $2,
					      NULL);
	}
	| '-' primary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      $2,
					      NULL);
	}
	| '~' primary_expr
	{
	  $$ = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      $2,
					      NULL);
	}
	;

primary_expr
	: scoped_name
	{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  $$ = idl_global->gen()->create_expr($1);
	}
	| literal
	| '(' const_expr ')'
	{
	  $$ = $2;
	}
	;

literal
	: INTEGER_LITERAL
	{
	  $$ = idl_global->gen()->create_expr($1);
	}
	| STRING_LITERAL
	{
	  $$ = idl_global->gen()->create_expr($1);
	}
	| CHARACTER_LITERAL
	{
	  $$ = idl_global->gen()->create_expr($1);
	}
	| FLOATING_PT_LITERAL
	{
	  $$ = idl_global->gen()->create_expr($1);
	}
	| TRUETOK
	{
	  $$ = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					    AST_Expression::EV_bool);
	}
	| FALSETOK
	{
	  $$ = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					    AST_Expression::EV_bool);
	}
	;

positive_int_expr :
	const_expr
	{
	    $1->evaluate(AST_Expression::EK_const);
	    $$ = idl_global->gen()->create_expr($1, AST_Expression::EV_ulong);
	}
	;

type_dcl
	: TYPEDEF
	  {
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
	  type_declarator
	| struct_type
	| union_type
	| enum_type
	;

type_declarator :
	type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
	at_least_one_declarator
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l;
	  FE_Declarator		*d = NULL;
	  AST_Typedef		*t = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclaratorsSeen);
	  /*
	   * Create a list of type renamings. Add them to the
	   * enclosing scope
	   */
	  if (s != NULL && $1 != NULL && $3 != NULL) {
	    l = new UTL_DecllistActiveIterator($3);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL) 
		continue;
              AST_Type * tp = d->compose($1);
              if (tp == NULL) 
		continue;
	      t = idl_global->gen()->create_typedef(tp, d->name(), p);
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
	}
	;

type_spec
	: simple_type_spec
	| constructed_type_spec
	;

simple_type_spec
	: base_type_spec
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
	| template_type_spec
	| scoped_name
	{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name($1, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error($1);
	  $$ = d;
	}
	;

base_type_spec
	: integer_type
	| floating_pt_type
	| char_type
	| boolean_type
	| octet_type
	| any_type
	;

template_type_spec
	: sequence_type_spec
	| string_type_spec
	| wstring_type_spec
	;

constructed_type_spec
	: struct_type
	| union_type
	| enum_type
	;

at_least_one_declarator :
	declarator declarators
	{
	  $$ = new UTL_DeclList($1, $2);
	}
	;

declarators
	: declarators
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
	  declarator
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if ($1 == NULL)
	    $$ = new UTL_DeclList($4, NULL);
	  else {
	    $1->nconc(new UTL_DeclList($4, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

declarator
	: simple_declarator
	| complex_declarator
	;

at_least_one_simple_declarator :
	simple_declarator simple_declarators
	{
	  $$ = new UTL_DeclList($1, $2);
	}
	;

simple_declarators
	: simple_declarators
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
	  simple_declarator
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if ($1 == NULL)
	    $$ = new UTL_DeclList($4, NULL);
	  else {
	    $1->nconc(new UTL_DeclList($4, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

simple_declarator :
	id
	{
	  $$ = new FE_Declarator(new UTL_ScopedName($1, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
	;

complex_declarator :
	array_declarator
	{
	  $$ = new FE_Declarator(new UTL_ScopedName($1->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 $1);
	}
	;

integer_type
	: signed_int
	| unsigned_int
	;

signed_int
	: LONG
	{
	  $$ = AST_Expression::EV_long;
	}
	| LONG LONG
	{
	  $$ = AST_Expression::EV_longlong;
        }
	| SHORT
	{
	  $$ = AST_Expression::EV_short;
	}
	;

unsigned_int
	: UNSIGNED LONG
	{
	  $$ = AST_Expression::EV_ulong;
	}
	| UNSIGNED LONG LONG
	{
	  $$ = AST_Expression::EV_ulonglong;
        }
	| UNSIGNED SHORT
	{
	  $$ = AST_Expression::EV_ushort;
	}
	;

floating_pt_type
	: DOUBLE
	{
	  $$ = AST_Expression::EV_double;
	}
	| FLOAT
	{
	  $$ = AST_Expression::EV_float;
	}
	| LONG DOUBLE
	{
	  $$ = AST_Expression::EV_longdouble;
        }
	;

char_type
	: CHAR
	{
	  $$ = AST_Expression::EV_char;
	}
	| WCHAR
	{
	  $$ = AST_Expression::EV_wchar;
        }
	;

octet_type
	: OCTET
	{ 
          $$ = AST_Expression::EV_octet;
	}
	;

boolean_type
	: BOOLEAN
	{ 
	  $$ = AST_Expression::EV_bool;
        }
	;

any_type
	: ANY
	{
	  $$ = AST_Expression::EV_any;
	}
	;

struct_type :
	STRUCT
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
	id
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName($3, NULL);
	  AST_Structure		*d = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_StructIDSeen);
	  /*
	   * Create a node representing a struct declaration. Add it
	   * to the enclosing scope
	   */
	  if (s != NULL) {
	    d = idl_global->gen()->create_structure(n, p);
	    (void) s->fe_add_structure(d);
	  }
	  /*
	   * Push the scope of the struct on the scopes stack
	   */
	  idl_global->scopes()->push(d);
	}
	'{'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
	at_least_one_member
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
	'}'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructQsSeen);
	  /*
	   * Done with this struct. Pop its scope off the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    $$ = NULL;
	  else {
	    $$ =
	      AST_Structure::narrow_from_scope(
				   idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	}
	;

at_least_one_member : member members ;

members
	: members member
	| /* EMPTY */
	;

member	:
	type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
	at_least_one_declarator
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
	';'
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l = NULL;
	  FE_Declarator		*d = NULL;
	  AST_Field		*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsCompleted);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if ($1 != NULL && AST_illegal_recursive_type($1))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, $1);
	  /*
	   * Create a node representing a struct or exception member
	   * Add it to the enclosing scope
	   */
	  else if (s != NULL && $1 != NULL && $3 != NULL) {
	    l = new UTL_DecllistActiveIterator($3);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL) 
		continue;
 	      AST_Type *tp = d->compose($1);
	      if (tp == NULL) 
		continue;
	      f = idl_global->gen()->create_field(tp, d->name(), p);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	}
	| error
	{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
	';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
	;

union_type :
	UNION
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
	id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
	SWITCH
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
	'('
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
	switch_type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
	')'
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName($3, NULL);
	  AST_Union		*u = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchCloseParSeen);
	  /*
	   * Create a node representing a union. Add it to its enclosing
	   * scope
	   */
	  if ($9 != NULL && s != NULL) {
 	    AST_ConcreteType    *tp = AST_ConcreteType::narrow_from_decl($9);
            if (tp == NULL) {
              idl_global->err()->not_a_type($9);
            } else {
	      u = idl_global->gen()->create_union(tp, n, p);
	      (void) s->fe_add_union(u);
 	    }
	  }
	  /*
	   * Push the scope of the union on the scopes stack
	   */
	  idl_global->scopes()->push(u);
	}
	'{'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
	at_least_one_case_branch
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
	'}'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionQsSeen);
	  /*
	   * Done with this union. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    $$ = NULL;
	  else {
	    $$ =
	      AST_Union::narrow_from_scope(
				idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	}
	;

switch_type_spec :
	integer_type
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
	| char_type
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
	| octet_type
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
	| boolean_type
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
	| enum_type
	| scoped_name
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*p = NULL;
	  AST_Typedef		*t = NULL;
	  long			found = I_FALSE;

	  /*
	   * The discriminator is a scoped name. Try to resolve to
	   * one of the scalar types or to an enum. Thread through
	   * typedef's to arrive at the base type at the end of the
	   * chain
	   */
	  if (s != NULL && (d = s->lookup_by_name($1, I_TRUE)) != NULL) {
	    while (!found) {
	      switch (d->node_type()) {
	      case AST_Decl::NT_enum:
		$$ = d;
		found = I_TRUE;
		break;
	      case AST_Decl::NT_pre_defined:
		p = AST_PredefinedType::narrow_from_decl(d);
		if (p != NULL) {
		  switch (p->pt()) {
		  case AST_PredefinedType::PT_long:
		  case AST_PredefinedType::PT_ulong:
		  case AST_PredefinedType::PT_longlong:
		  case AST_PredefinedType::PT_ulonglong:
		  case AST_PredefinedType::PT_short:
		  case AST_PredefinedType::PT_char:
		  case AST_PredefinedType::PT_wchar:
		  case AST_PredefinedType::PT_octet:
		  case AST_PredefinedType::PT_boolean:
		    $$ = p;
		    found = I_TRUE;
		    break;
		  default:
		    $$ = NULL;
		    found = I_TRUE;
		    break;
		  }
		}
		break;
	      case AST_Decl::NT_typedef:
		t = AST_Typedef::narrow_from_decl(d);
		if (t != NULL) d = t->base_type();
		break;
	      default:
		$$ = NULL;
		found = I_TRUE;
		break;
	      }
	    }
	  } else
	    $$ = NULL;

	  if ($$ == NULL)
	    idl_global->err()->lookup_error($1);
	}
	;

at_least_one_case_branch : case_branch case_branches ;

case_branches
	: case_branches case_branch
	| /* empty */
	;

case_branch :
	at_least_one_case_label
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
	element_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
	';'
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = $3;

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && $1 != NULL && $3 != NULL) {
	    l = new UTL_LabellistActiveIterator($1);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      b = idl_global->gen()->create_union_branch(d,
						      f->field_type(),
						      f->name(),
						      f->pragmas());
	      (void) s->fe_add_union_branch(b);
	    }
	    delete l;
	  }
	}
	| error
	{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
	';'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
	;

at_least_one_case_label :
	case_label case_labels
	{
	  $$ = new UTL_LabelList($1, $2);
	}
	;

case_labels
	: case_labels case_label
	{
	  if ($1 == NULL)
	    $$ = new UTL_LabelList($2, NULL);
	  else {
	    $1->nconc(new UTL_LabelList($2, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

case_label
	: DEFAULT
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
	  ':'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  $$ = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
	| CASE
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
	  const_expr
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
	':'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  $$ = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     $3);
	}
	;

element_spec :
	type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
	declarator
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemDeclSeen);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if ($1 != NULL && AST_illegal_recursive_type($1))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, $1);
	  /*
	   * Create a field in a union branch
	   */
	  else if ($1 == NULL || $3 == NULL)
	    $$ = NULL;
	  else {
	    AST_Type *tp = $3->compose($1);
	    if (tp == NULL)
	      $$ = NULL;
 	    else
	      $$ = idl_global->gen()->create_field(tp,
			        		   $3->name(),
			        		   idl_global->pragmas());
	  }
	}
	;

enum_type :
	ENUM
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
	id
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName($3, NULL);
	  AST_Enum		*e = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumIDSeen);
	  /*
	   * Create a node representing an enum and add it to its
	   * enclosing scope
	   */
	  if (s != NULL) {
	    e = idl_global->gen()->create_enum(n, p);
	    /*
	     * Add it to its defining scope
	     */
	    (void) s->fe_add_enum(e);
	  }
	  /*
	   * Push the enum scope on the scopes stack
	   */
	  idl_global->scopes()->push(e);
	}
	'{'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
	at_least_one_enumerator
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
	'}'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumQsSeen);
	  /*
	   * Done with this enum. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    $$ = NULL;
	  else {
	    $$ = AST_Enum::narrow_from_scope(idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	}
	;

at_least_one_enumerator : enumerator enumerators ;

enumerators
	: enumerators
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
	  enumerator
	| /* EMPTY */
	;

enumerator :
	IDENTIFIER
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier($1, 1, 0, I_FALSE), NULL);
	  AST_EnumVal		*e = NULL;
	  AST_Enum		*c = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  /*
	   * Create a node representing one enumerator in an enum
	   * Add it to the enclosing scope (the enum scope)
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_enum) {
	    c = AST_Enum::narrow_from_scope(s);
	    if (c != NULL) 
	      e = idl_global->gen()->create_enum_val(c->next_enum_val(), n, p);
	    (void) s->fe_add_enum_val(e);
	  }
	}
	;

sequence_type_spec
        : seq_head
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
	positive_int_expr
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
	  '>'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
	  /*
	   * Create a node representing a sequence
	   */
	  if ($4 == NULL || $4->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error($4, AST_Expression::EV_ulong);
	    $$ = NULL;
	  } else if ($1 == NULL) {
	    $$ = NULL;
	  } else {
 	    AST_Type *tp = AST_Type::narrow_from_decl($1); 
	    if (tp == NULL) 
	      $$ = NULL;
	    else {
	      $$ = idl_global->gen()->create_sequence($4, tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl($$));
 	    }
	  }
	}
	| seq_head
	  '>'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
	  /*
	   * Create a node representing a sequence
	   */
	  if ($1 == NULL)
	    $$ = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl($1);
	    if (tp == NULL)
	      $$ = NULL;
            else {
	      $$ =
	        idl_global->gen()->create_sequence(
		  	     idl_global->gen()->create_expr((unsigned long) 0),
			     tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl($$));
	    }
	  }
	}
	;

seq_head:
	SEQUENCE
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
	'<'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
	simple_type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  $$ = $5;
        }
	;

string_type_spec
	: string_head
	  '<'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
	positive_int_expr
	{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
	'>'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if ($4 == NULL || $4->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error($4, AST_Expression::EV_ulong);
	    $$ = NULL;
	  } else {
	    $$ = idl_global->gen()->create_string($4);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl($$));
	  }
	}
	| string_head
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  $$ =
	    idl_global->gen()->create_string(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl($$));
	}
	;

string_head:
	STRING
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
	;

wstring_type_spec
	: wstring_head
	  '<'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
	positive_int_expr
	{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
	'>'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if ($4 == NULL || $4->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error($4, AST_Expression::EV_ulong);
	    $$ = NULL;
	  } else {
	    $$ = idl_global->gen()->create_wstring($4);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl($$));
	  }
	}
	| wstring_head
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  $$ =
	    idl_global->gen()->create_wstring(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl($$));
	}
	;

wstring_head:
	WSTRING
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
	;

array_declarator :
	id
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
	at_least_one_array_dim
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
	  /*
	   * Create a node representing an array
	   */
	  if ($3 != NULL) {
	     $$ = idl_global->gen()->create_array(new UTL_ScopedName($1, NULL),
						  $3->length(), $3);
	  }
	}
	;

at_least_one_array_dim :
	array_dim array_dims
	{
	  $$ = new UTL_ExprList($1, $2);
	}
	;

array_dims
	: array_dims array_dim
	{
	  if ($1 == NULL)
	    $$ = new UTL_ExprList($2, NULL);
	  else {
	    $1->nconc(new UTL_ExprList($2, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

array_dim :
	'['
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
	positive_int_expr
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
	']'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimQsSeen);
	  /*
	   * Array dimensions are expressions which must be coerced to
	   * positive integers
	   */
	  if ($3 == NULL || $3->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error($3, AST_Expression::EV_ulong);
	    $$ = NULL;
	  } else
	    $$ = $3;
	}
	;

attribute:
	opt_readonly
	ATTRIBUTE
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
	param_type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
	at_least_one_simple_declarator
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l = NULL;
	  AST_Attribute		*a = NULL;
	  FE_Declarator		*d = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrCompleted);
	  /*
	   * Create nodes representing attributes and add them to the
	   * enclosing scope
	   */
	  if (s != NULL && $4 != NULL && $6 != NULL) {
	    l = new UTL_DecllistActiveIterator($6);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      AST_Type *tp = d->compose($4);
 	      if (tp == NULL)
	 	continue;
	      a = idl_global->gen()->create_attribute($1, tp, d->name(), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
	}	    
	;

opt_readonly
	: READONLY
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  $$ = I_TRUE;
	}
	| /* EMPTY */
	{
	  $$ = I_FALSE;
	}
	;

exception :
	EXCEPTION
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
	id
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName($3, NULL);
	  AST_Exception		*e = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptIDSeen);
	  /*
	   * Create a node representing an exception and add it to
	   * the enclosing scope
	   */
	  if (s != NULL) {
	    e = idl_global->gen()->create_exception(n, p);
	    (void) s->fe_add_exception(e);
	  }
	  /*
	   * Push the exception scope on the scope stack
	   */
	  idl_global->scopes()->push(e);
	}
	'{'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
	members
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
	'}'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
	  idl_global->scopes()->pop();
	}
	;

operation :
	opt_op_attribute
	op_type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
	IDENTIFIER
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier($4, 1, 0, I_FALSE), NULL);
	  AST_Operation		*o = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpIDSeen);
	  /*
	   * Create a node representing an operation on an interface
	   * and add it to its enclosing scope
	   */
	  if (s != NULL && $2 != NULL) {
	    AST_Type *tp = AST_Type::narrow_from_decl($2);
            if (tp == NULL) {
              idl_global->err()->not_a_type($2);
            } else if (tp->node_type() == AST_Decl::NT_except) {
              idl_global->err()->not_a_type($2);
            } else {
	      o = idl_global->gen()->create_operation(tp, $1, n, p);
	      (void) s->fe_add_operation(o);
	    }
	  }
	  /*
	   * Push the operation scope onto the scopes stack
	   */
	  idl_global->scopes()->push(o);
	}
	parameter_list
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
	opt_raises
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
	opt_context
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Operation		*o = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpCompleted);
	  /*
	   * Add exceptions and context to the operation
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_op) {
	    o = AST_Operation::narrow_from_scope(s);

	    if ($8 != NULL && o != NULL)
	      (void) o->fe_add_exceptions($8);
	    if ($10 != NULL)
	      (void) o->fe_add_context($10);
	  }
	  /*
	   * Done with this operation. Pop its scope from the scopes stack
	   */
	  idl_global->scopes()->pop();
	}
	;

opt_op_attribute
	: ONEWAY
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  $$ = AST_Operation::OP_oneway;
	}
	| IDEMPOTENT
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  $$ = AST_Operation::OP_idempotent;
	}
	| /* EMPTY */
	{
	  $$ = AST_Operation::OP_noflags;
	}
	;

op_type_spec
	: param_type_spec
	| VOID
	{
	  $$ =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
	;

parameter_list
	: '('
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
	  ')'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
	| '('
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
	  at_least_one_parameter
	  ')'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
	;

at_least_one_parameter : parameter parameters ;

parameters
	: parameters
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
	  parameter
	| /* EMPTY */
	;

parameter :
	direction
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
	param_type_spec
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
	declarator
	{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Argument		*a = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
	  /*
	   * Create a node representing an argument to an operation
	   * Add it to the enclosing scope (the operation scope)
	   */
	  if ($3 != NULL && $5 != NULL && s != NULL) {
	    AST_Type *tp = $5->compose($3);
	    if (tp != NULL) {
	      a = idl_global->gen()->create_argument($1, tp, $5->name(), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	}	    
	;

param_type_spec
        : base_type_spec
	{
	  $$ = idl_global->scopes()->bottom()->lookup_primitive_type($1);
	}
        | string_type_spec
        | scoped_name
	{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name($1, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error($1);
	  $$ = d;
	}
	;

direction
	: IN
	{
	  $$ = AST_Argument::dir_IN;
	}
	| OUT
	{
	  $$ = AST_Argument::dir_OUT;
	}
	| INOUT
	{
	  $$ = AST_Argument::dir_INOUT;
	}
	;

opt_raises
	: RAISES
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
	  '('
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
	  at_least_one_scoped_name
	  ')'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  $$ = $5;
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

opt_context
	: CONTEXT
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
	  '('
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
	  at_least_one_string_literal
	  ')'
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  $$ = $5;
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

at_least_one_string_literal :
	STRING_LITERAL string_literals
	{
	  $$ = new UTL_StrList($1, $2);
	}
	;

string_literals
	: string_literals
	  ','
	{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
	  STRING_LITERAL
	{
	  if ($1 == NULL)
	    $$ = new UTL_StrList($4, NULL);
	  else {
	    $1->nconc(new UTL_StrList($4, NULL));
	    $$ = $1;
	  }
	}
	| /* EMPTY */
	{
	  $$ = NULL;
	}
	;

%%
/* programs */

/*
 * ???
 */
int
yywrap()
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
void
yyerror(char *)
{
}
