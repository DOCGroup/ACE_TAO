/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Interpreter.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_TCL_INTERPRETER_H
#define TAO_TCL_INTERPRETER_H
#include "ace/pre.h"

#include "Constraint_Nodes.h"
#include "Constraint_Visitors.h"
#include "orbsvcs/Trader/trading_export.h"

class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;

class TAO_Trading_Export TAO_Interpreter
{
  // = TITLE
  //      TAO_Interpreter is the superclass for all interpreters. Its
  //      build tree method invokes the yacc parser to parse a constraint
  //      or preference string.
protected:
  // = Initialization and termination methods.
  TAO_Interpreter (void);
  // Constructor.

  ~TAO_Interpreter (void);
  // Destructor.

  int build_tree (const char* preferences);
  // Using the Yacc generated parser, construct an expression tree
  // representing <constraints> from the tokens returned by it.

  static int is_empty_string (const char* str);

  TAO_Constraint* root_;
  // The root of the expression tree, not equal to null if build_tree
  // successfully builds a tree from the constraints.
private:
  static TAO_SYNCH_MUTEX parserMutex__;
  // This mutex protects the <build_tree> method from reentrance.
};


// Functions we need for parsing.
extern int yyparse (void);
extern void yyrestart (FILE*);
extern int yylex (void);

// Have yylex read from the constraint string, not from stdin.
#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = TAO_Lex_String_Input::copy_into(b, ms))

#undef yyerror
#define yyerror(x)

class TAO_Lex_String_Input
{
  // = TITLE
  //   Have Lex read from a string and not from stdin. Essentially,
  //   the interpreter needs to call yylex() until EOF, and call
  //   TAO_Lex_String_Input::reset() with the new string, prior to
  //   calling yyparse.
public:
  static void reset (char* input_string);
  // Reset the lex input.

  static int copy_into (char* buf, int max_size);
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

#include "ace/post.h"
#endif /* TAO_TCL_INTERPRETER_H */
