/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Constraint.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_CONSTRAINT_H
#define TAO_CONSTRAINT_H

#include "Constraint_Nodes.h"

// Functions we need for parsing.
extern int yyparse(void);
extern void yyrestart(FILE*);
extern int yylex(void);

// Have yylex read from the constraint string, not from stdin.
#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = TAO_Lex_String_Input::copy_into(b, ms))

#undef yyerror
#define yyerror(x) 

class TAO_Lex_String_Input
// = TITLE
//   Stupid hack to have Lex read from a string and not from
//   stdin. Essentially, the interpreter needs to call yylex() until
//   EOF, and call TAO_Lex_String_Input::reset() with the new string,
//   prior to calling yyparse.
{
public:  

  static void reset(char* input_string);
  // Reset the lex input.
  
  static int copy_into(char* buf, int max_size);
  // Method lex will call to read from the input string.
  
private:

  static char* string_;
  static char* current_;
  static char* end_;
  // Pointers to keep track of the input string.
};

// The union used by lex and yacc to build the Abstract Syntax Tree.
typedef union
{
  TAO_Constraint* constraint_;	
} YYSTYPE;

extern YYSTYPE yylval;
extern YYSTYPE yyval;

#endif /* CONSTRAINT_H */
