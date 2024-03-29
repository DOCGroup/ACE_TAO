// -*- C++ -*-

//=============================================================================
/**
 *  @file    Interpreter.h
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TCL_INTERPRETER_H
#define TAO_TCL_INTERPRETER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Constraint_Nodes.h"
#include "orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/Trader/trading_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;

/**
 * @class TAO_Interpreter
 *
 * @brief TAO_Interpreter is the superclass for all interpreters. Its
 * build tree method invokes the yacc parser to parse a constraint
 * or preference string.
 */
class TAO_Trading_Serv_Export TAO_Interpreter
{
protected:
  /// Constructor.
  TAO_Interpreter ();

  /// Destructor.
  ~TAO_Interpreter ();

  /// Using the Yacc generated parser, construct an expression tree
  /// representing <constraints> from the tokens returned by it.
  int build_tree (const char* preferences);

  static int is_empty_string (const char* str);

  /// The root of the expression tree, not equal to null if build_tree
  /// successfully builds a tree from the constraints.
  TAO_Constraint* root_;
private:
  /// This mutex protects the <build_tree> method from reentrance.
  static TAO_SYNCH_MUTEX parserMutex__;
};

// Functions we need for parsing.
extern int yyparse ();
extern void yyrestart (FILE*);
extern int yylex ();

// Have yylex read from the constraint string, not from stdin.
#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = TAO_Lex_String_Input::copy_into(b, ms))

#undef yyerror
#define yyerror(x)

/**
 * @class TAO_Lex_String_Input
 *
 * @brief Have Lex read from a string and not from stdin. Essentially,
 * the interpreter needs to call yylex() until EOF, and call
 * TAO_Lex_String_Input::reset() with the new string, prior to
 * calling yyparse.
 */
class TAO_Lex_String_Input
{
public:
  /// Reset the lex input.
  static void reset (char* input_string);

  /// Method lex will call to read from the input string.
  static int copy_into (char* buf, int max_size);

private:
  /// Pointers to keep track of the input string.
  static char* string_;
  static char* current_;
  static char* end_;
};

// The union used by lex and yacc to build the Abstract Syntax Tree.
#define YYSTYPE_IS_DECLARED
typedef union
{
  TAO_Constraint* constraint_;
} YYSTYPE;

extern YYSTYPE yylval;
extern YYSTYPE yyval;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_TCL_INTERPRETER_H */
