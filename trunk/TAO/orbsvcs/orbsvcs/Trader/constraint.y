%{
  // $Id$
// ========================================================================
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    constraint.y
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ace/OS.h"
#include "Constraint_Interpreter.h"           
#include "Constraint_Nodes.h"

//#define YYDEBUG 1
%}

%token TAO_GT TAO_GE TAO_LT TAO_LE TAO_EQ TAO_NE TAO_EXIST
%token TAO_AND TAO_OR TAO_NOT TAO_IN TAO_TWIDDLE TAO_BOOLEAN
%token TAO_PLUS TAO_MINUS TAO_MULT TAO_DIV TAO_UMINUS TAO_NUMBER 
%token TAO_RPAREN TAO_LPAREN TAO_IDENT TAO_STRING TAO_UNKNOWN
%token TAO_UNSIGNED TAO_SIGNED TAO_DOUBLE TAO_CONSTRAINT TAO_SEQUENCE 
%token TAO_WITH TAO_MAX TAO_MIN TAO_FIRST TAO_RANDOM

%start constraint

%type <constraint_> TAO_IDENT TAO_NUMBER TAO_STRING TAO_BOOLEAN
%type <constraint_> constraint preference bool_or bool_and bool_compare 
%type <constraint_> expr_in expr_twiddle expr term factor_not factor

%%

constraint: bool_or
	{ $$ = new TAO_Unary_Constraint(TAO_CONSTRAINT, $1); }
        | preference
        { $$ = $1; }
	;

preference:     TAO_MIN bool_or
{ $$ = new TAO_Unary_Constraint(TAO_MIN, $2); }
        |       TAO_MAX bool_or
{ $$ = new TAO_Unary_Constraint(TAO_MAX, $2); }
        |       TAO_WITH bool_or
{ $$ = new TAO_Unary_Constraint(TAO_WITH, $2); }
        |       TAO_FIRST
{ $$ = new TAO_Noop_Constraint(TAO_FIRST); }
        |       TAO_RANDOM
{ $$ = new TAO_Noop_Constraint(TAO_RANDOM); }
        ;

bool_or:	bool_or TAO_OR bool_and
		{ $$ = new TAO_Binary_Constraint(TAO_OR, $1, $3); }
	|	bool_and
		{ $$ = $1; }
	;

bool_and:	bool_and TAO_AND bool_compare
		{ $$ = new TAO_Binary_Constraint(TAO_AND, $1, $3); }
	|	bool_compare
		{ $$ = $1; }
	;

bool_compare:	expr_in TAO_EQ expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_EQ, $1, $3); }
	|	expr_in TAO_NE expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_NE, $1, $3); }
	|	expr_in TAO_GT expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_GT, $1, $3); }
	|	expr_in TAO_GE expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_GE, $1, $3); }
	|	expr_in	TAO_LT expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_LT, $1, $3); }
	|	expr_in TAO_LE expr_in
	{ $$ = new TAO_Binary_Constraint(TAO_LE, $1, $3); }
	|	expr_in
	{ $$ = $1; }	
	;

expr_in:	expr_twiddle TAO_IN TAO_IDENT
	{ $$ = new TAO_Binary_Constraint(TAO_IN, $1, $3); }
	|	expr_twiddle
	{ $$ = $1; }
	;

expr_twiddle:	expr TAO_TWIDDLE expr
	{ $$ = new TAO_Binary_Constraint(TAO_TWIDDLE, $1, $3); }
	|	expr
	{ $$ = $1; }
	;

expr:		expr TAO_PLUS term
	{ $$ = new TAO_Binary_Constraint(TAO_PLUS, $1, $3); }
	|	expr TAO_MINUS term
	{ $$ = new TAO_Binary_Constraint(TAO_MINUS, $1, $3); }
	|	term
	{ $$ = $1; }
	;

term:		term TAO_MULT factor_not
	{ $$ = new TAO_Binary_Constraint(TAO_MULT, $1, $3); }
	|	term TAO_DIV factor_not
	{ $$ = new TAO_Binary_Constraint(TAO_DIV, $1, $3); }
	|	factor_not
	{ $$ = $1; }
	;

factor_not:	TAO_NOT factor
	{ $$ = new TAO_Unary_Constraint(TAO_NOT, $2); }
	|	factor
	{ $$ = $1; }
	;

factor:		TAO_LPAREN bool_or TAO_RPAREN
	{ $$ = $2; }
	|	TAO_EXIST TAO_IDENT
	{ $$ = new TAO_Unary_Constraint(TAO_EXIST, $2); }
	|	TAO_IDENT
	{ $$ = $1; }
	|	TAO_NUMBER
	{ $$ = $1; }
	|	TAO_MINUS TAO_NUMBER
	{ $$ = new TAO_Unary_Constraint(TAO_UMINUS, $2); }
	|	TAO_STRING
	{ $$ = $1; }
	|	TAO_BOOLEAN
	{ $$ = $1; }
	;

%%

//extern int yydebug = 1;
