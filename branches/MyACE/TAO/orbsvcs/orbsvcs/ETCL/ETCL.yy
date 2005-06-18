%{
// $Id$
// ========================================================================
//
// = LIBRARY
//   orbsvcs / Extended Trader Constraint Language parser.
// 
// = FILENAME
//   ETCL.yy
//
// = AUTHOR
//   Carlos O'Ryan <coryan@uci.edu> based on previous work by
//   Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ETCL_y.h"
#include "ETCL_Constraint.h"
#include "ETCL_Interpreter.h"

extern int yylex (void);

static void yyerror (const char *)
{
  // @@ TODO
  // Ignore error messages
}

%}

%token TAO_ETCL_GT 
%token TAO_ETCL_GE 
%token TAO_ETCL_LT 
%token TAO_ETCL_LE
%token TAO_ETCL_EQ 
%token TAO_ETCL_NE
%token TAO_ETCL_EXIST 
%token TAO_ETCL_DEFAULT
%token TAO_ETCL_AND 
%token TAO_ETCL_OR 
%token TAO_ETCL_NOT
%token TAO_ETCL_IN 
%token TAO_ETCL_TWIDDLE
%token TAO_ETCL_BOOLEAN
%token TAO_ETCL_PLUS 
%token TAO_ETCL_MINUS 
%token TAO_ETCL_MULT 
%token TAO_ETCL_DIV
%token TAO_ETCL_UMINUS
%token TAO_ETCL_INTEGER 
%token TAO_ETCL_FLOAT  
%token TAO_ETCL_STRING
%token TAO_ETCL_RPAREN 
%token TAO_ETCL_LPAREN
%token TAO_ETCL_RBRA 
%token TAO_ETCL_LBRA
%token TAO_ETCL_IDENT
%token TAO_ETCL_UNSIGNED 
%token TAO_ETCL_SIGNED 
%token TAO_ETCL_DOUBLE
%token TAO_ETCL_CONSTRAINT 
%token TAO_ETCL_COMPONENT 
%token TAO_ETCL_WITH 
%token TAO_ETCL_MAX 
%token TAO_ETCL_MIN
%token TAO_ETCL_FIRST 
%token TAO_ETCL_RANDOM
%token TAO_ETCL_DOLLAR 
%token TAO_ETCL_DOT
%token TAO_ETCL_DISCRIMINANT 
%token TAO_ETCL_LENGTH
%token TAO_ETCL_TYPE_ID 
%token TAO_ETCL_REPOS_ID


%type <constraint> TAO_ETCL_IDENT
%type <constraint> TAO_ETCL_BOOLEAN
%type <constraint> TAO_ETCL_STRING
%type <constraint> TAO_ETCL_FLOAT
%type <constraint> TAO_ETCL_INTEGER
%type <constraint> expr_in
%type <constraint> constraint preference bool_or bool_and bool_compare 
%type <constraint> expr_in expr_twiddle expr term factor_not factor
%type <constraint> union_pos union_val component_array
%type <constraint> component_array component_assoc component_pos
%type <constraint> component_dot component_ext component

%start constraint

%%

constraint: bool_or
        | preference
	;

preference:     TAO_ETCL_MIN bool_or
        { $$ = new TAO_ETCL_Preference (TAO_ETCL_MIN, $2); }
        |       TAO_ETCL_MAX bool_or
        { $$ = new TAO_ETCL_Preference (TAO_ETCL_MAX, $2); }
        |       TAO_ETCL_WITH bool_or
        { $$ = new TAO_ETCL_Preference (TAO_ETCL_WITH, $2); }
        |       TAO_ETCL_FIRST
        { $$ = new TAO_ETCL_Preference (TAO_ETCL_FIRST); }
        |       TAO_ETCL_RANDOM
        { $$ = new TAO_ETCL_Preference (TAO_ETCL_RANDOM); }
        ;

bool_or:	bool_or TAO_ETCL_OR bool_and
		{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_OR, $1, $3); }
	|	bool_and
	;

bool_and:	bool_and TAO_ETCL_AND bool_compare
		{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_AND, $1, $3); }
	|	bool_compare
	;

bool_compare:	expr_in TAO_ETCL_EQ expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_EQ, $1, $3); }
	|	expr_in TAO_ETCL_NE expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_NE, $1, $3); }
	|	expr_in TAO_ETCL_GT expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_GT, $1, $3); }
	|	expr_in TAO_ETCL_GE expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_GE, $1, $3); }
	|	expr_in	TAO_ETCL_LT expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_LT, $1, $3); }
	|	expr_in TAO_ETCL_LE expr_in
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_LE, $1, $3); }
	|	expr_in
	;

expr_in:	expr_twiddle TAO_ETCL_IN component
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_IN, $1, $3); }
	|	expr_twiddle TAO_ETCL_IN TAO_ETCL_DOLLAR component
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_IN, $1, $4); }
	|	expr_twiddle
	;

expr_twiddle:	expr TAO_ETCL_TWIDDLE expr
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_TWIDDLE, $1, $3); }
	|	expr
	;

expr:		expr TAO_ETCL_PLUS term
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_PLUS, $1, $3); }
	|	expr TAO_ETCL_MINUS term
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_MINUS, $1, $3); }
	|	term
	;

term:		term TAO_ETCL_MULT factor_not
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_MULT, $1, $3); }
	|	term TAO_ETCL_DIV factor_not
	{ $$ = new TAO_ETCL_Binary_Expr (TAO_ETCL_DIV, $1, $3); }
	|	factor_not
	;

factor_not:	TAO_ETCL_NOT factor
	{ $$ = new TAO_ETCL_Unary_Expr (TAO_ETCL_NOT, $2); }
	|	factor
	;

factor:		TAO_ETCL_LPAREN bool_or TAO_ETCL_RPAREN
	{ $$ = $2; }
	|	TAO_ETCL_INTEGER
	{ $$ = $1; }
	|	TAO_ETCL_PLUS TAO_ETCL_INTEGER
	{ $$ = new TAO_ETCL_Unary_Expr (TAO_ETCL_PLUS, $2); }
	|	TAO_ETCL_MINUS TAO_ETCL_INTEGER
	{ $$ = new TAO_ETCL_Unary_Expr (TAO_ETCL_MINUS, $2); }
	|	TAO_ETCL_FLOAT
	{ $$ = $1; }
	|	TAO_ETCL_PLUS TAO_ETCL_FLOAT
	{ $$ = new TAO_ETCL_Unary_Expr (TAO_ETCL_PLUS, $2); }
	|	TAO_ETCL_MINUS TAO_ETCL_FLOAT
	{ $$ = new TAO_ETCL_Unary_Expr (TAO_ETCL_MINUS, $2); }
	|	TAO_ETCL_STRING
	{ $$ = $1; }
	|	TAO_ETCL_BOOLEAN
	{ $$ = $1; }
	|	TAO_ETCL_EXIST TAO_ETCL_IDENT
	{ $$ = new TAO_ETCL_Exist ($2); }
	|	TAO_ETCL_EXIST TAO_ETCL_DOLLAR component
	{ $$ = new TAO_ETCL_Exist ($3); }
	|	TAO_ETCL_DEFAULT TAO_ETCL_DOLLAR component
	{ $$ = new TAO_ETCL_Default ($3); }
	|	TAO_ETCL_DOLLAR component
	{ $$ = new TAO_ETCL_Eval ($2); }
	| 	TAO_ETCL_IDENT
	{ $$ = $1; }
	;

component:	/* empty */
	{ $$ = 0; }
	| TAO_ETCL_DOT component_dot
	{ $$ = new TAO_ETCL_Dot ($2); }

	| TAO_ETCL_IDENT component_ext
	{ $$ = new TAO_ETCL_Component ($1, $2); }

	| component_array
	| component_assoc
	;

component_ext:	/* empty */
	{ $$ = 0; }
	| TAO_ETCL_DOT component_dot
	{ $$ = new TAO_ETCL_Dot ($2); }

	| component_array
	| component_assoc
	;

component_dot:  TAO_ETCL_IDENT component_ext
	{ $$ = new TAO_ETCL_Component ($1, $2); }
	| TAO_ETCL_LENGTH
	{ $$ = new TAO_ETCL_Special (TAO_ETCL_LENGTH); }
	| TAO_ETCL_DISCRIMINANT
	{ $$ = new TAO_ETCL_Special (TAO_ETCL_DISCRIMINANT); }
	| TAO_ETCL_TYPE_ID
	{ $$ = new TAO_ETCL_Special (TAO_ETCL_TYPE_ID); }
	| TAO_ETCL_REPOS_ID
	{ $$ = new TAO_ETCL_Special (TAO_ETCL_REPOS_ID); }
	| component_pos
	| union_pos
	;

component_array:  TAO_ETCL_LBRA TAO_ETCL_INTEGER TAO_ETCL_RBRA component_ext
	{ $$ = new TAO_ETCL_Component_Array ($2, $4); }
	;

component_assoc:  TAO_ETCL_LPAREN TAO_ETCL_IDENT TAO_ETCL_RPAREN component_ext
	{ $$ = new TAO_ETCL_Component_Assoc ($2, $4); }
	;

component_pos:  TAO_ETCL_INTEGER component_ext
	{ $$ = new TAO_ETCL_Component_Pos ($1, $2); }
	;

union_pos:  TAO_ETCL_LPAREN union_val TAO_ETCL_RPAREN component_ext
	{ $$ = new TAO_ETCL_Union_Pos ($2, $4); }
	;

union_val:  /* empty */
	{ $$ = 0; }
	| TAO_ETCL_INTEGER
	{ $$ = new TAO_ETCL_Union_Value (+1, $1); }
	| TAO_ETCL_PLUS TAO_ETCL_INTEGER
	{ $$ = new TAO_ETCL_Union_Value (+1, $2); }
	| TAO_ETCL_MINUS TAO_ETCL_INTEGER
	{ $$ = new TAO_ETCL_Union_Value (-1, $2); }
	| TAO_ETCL_STRING
	{ $$ = new TAO_ETCL_Union_Value ($1); }
	;

%%

