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

#include "Constraint.h"
#include "Constraint_Nodes.h"

//#define YYDEBUG 1
%}

%token GT GE LT LE EQ NE EXIST AND OR NOT IN TWIDDLE BOOLEAN
%token PLUS MINUS MULT DIV UMINUS NUMBER 
%token RPAREN LPAREN
%token IDENT STRING 
%token UNSIGNED_INTEGER SIGNED_INTEGER DOUBLE CONSTRAINT SEQUENCE UNKNOWN
%token WITH MAX MIN FIRST RANDOM

%start constraint

%type <constraint_> IDENT NUMBER STRING BOOLEAN
%type <constraint_> constraint preference bool_or bool_and bool_compare 
%type <constraint_> expr_in expr_twiddle expr term factor_not factor

%%

constraint: bool_or
	{ $$ = new ACE_Unary_Constraint(CONSTRAINT, $1); }
        | preference
        { $$ = $1; }
	;

preference:     MIN bool_or
{ $$ = new ACE_Unary_Constraint(MIN, $2); }
        |       MAX bool_or
{ $$ = new ACE_Unary_Constraint(MAX, $2); }
        |       WITH bool_or
{ $$ = new ACE_Unary_Constraint(WITH, $2); }
        |       FIRST
{ $$ = new ACE_Noop_Constraint(FIRST); }
        |       RANDOM
{ $$ = new ACE_Noop_Constraint(RANDOM); }
        ;

bool_or:	bool_or OR bool_and
		{ $$ = new ACE_Binary_Constraint(OR, $1, $3); }
	|	bool_and
		{ $$ = $1; }
	;

bool_and:	bool_and AND bool_compare
		{ $$ = new ACE_Binary_Constraint(AND, $1, $3); }
	|	bool_compare
		{ $$ = $1; }
	;

bool_compare:	expr_in EQ expr_in
	{ $$ = new ACE_Binary_Constraint(EQ, $1, $3); }
	|	expr_in NE expr_in
	{ $$ = new ACE_Binary_Constraint(NE, $1, $3); }
	|	expr_in GT expr_in
	{ $$ = new ACE_Binary_Constraint(GT, $1, $3); }
	|	expr_in GE expr_in
	{ $$ = new ACE_Binary_Constraint(GE, $1, $3); }
	|	expr_in	LT expr_in
	{ $$ = new ACE_Binary_Constraint(LT, $1, $3); }
	|	expr_in LE expr_in
	{ $$ = new ACE_Binary_Constraint(LE, $1, $3); }
	|	expr_in
	{ $$ = $1; }	
	;

expr_in:	expr_twiddle IN IDENT
	{ $$ = new ACE_Binary_Constraint(IN, $1, $3); }
	|	expr_twiddle
	{ $$ = $1; }
	;

expr_twiddle:	expr TWIDDLE expr
	{ $$ = new ACE_Binary_Constraint(TWIDDLE, $1, $3); }
	|	expr
	{ $$ = $1; }
	;

expr:		expr PLUS term
	{ $$ = new ACE_Binary_Constraint(PLUS, $1, $3); }
	|	expr MINUS term
	{ $$ = new ACE_Binary_Constraint(MINUS, $1, $3); }
	|	term
	{ $$ = $1; }
	;

term:		term MULT factor_not
	{ $$ = new ACE_Binary_Constraint(MULT, $1, $3); }
	|	term DIV factor_not
	{ $$ = new ACE_Binary_Constraint(DIV, $1, $3); }
	|	factor_not
	{ $$ = $1; }
	;

factor_not:	NOT factor
	{ $$ = new ACE_Unary_Constraint(NOT, $2); }
	|	factor
	{ $$ = $1; }
	;

factor:		LPAREN bool_or RPAREN
	{ $$ = $2; }
	|	EXIST IDENT
	{ $$ = new ACE_Unary_Constraint(EXIST, $2); }
	|	IDENT
	{ $$ = $1; }
	|	NUMBER
	{ $$ = $1; }
	|	MINUS NUMBER
	{ $$ = new ACE_Unary_Constraint(UMINUS, $2); }
	|	STRING
	{ $$ = $1; }
	|	BOOLEAN
	{ $$ = $1; }
	;

%%

//extern int yydebug = 1;
