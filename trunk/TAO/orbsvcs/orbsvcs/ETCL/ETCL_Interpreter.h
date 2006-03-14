// -*- C++ -*-

//=============================================================================
/**
 *  @file    ETCL_Interpreter.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu> based on previous work by
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ETCL_INTERPRETER_H
#define TAO_ETCL_INTERPRETER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/ETCL/etcl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "ace/Thread_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ETCL_Constraint;

/**
 * @class TAO_ETCL_Interpreter
 *
 * @brief TAO_ETCL_Interpreter is the superclass for all ETCL interpreters.
 * Its build tree method invokes the yacc parser to parse a constraint
 * or preference string.
 */
class TAO_ETCL_Export TAO_ETCL_Interpreter
{
protected:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_ETCL_Interpreter (void);

  /// Destructor.
  ~TAO_ETCL_Interpreter (void);

  /// Using the Yacc generated parser, construct an expression tree
  /// representing <constraints> from the tokens returned by it.
  int build_tree (const char* constraints);

  static int is_empty_string (const char* str);

  /// The root of the expression tree, not equal to null if build_tree
  /// successfully builds a tree from the constraints.
  TAO_ETCL_Constraint* root_;
private:
  /// This mutex protects the <build_tree> method from reentrance.
  static TAO_SYNCH_MUTEX parserMutex__;
};


// Functions we need for parsing.
extern int yyparse (void);
extern void yyrestart (FILE*);
extern int yylex (void);

// Have yylex read from the constraint string, not from stdin.
#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = TAO_Lex_String_Input::copy_into(b, ms))

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

/// The union used by lex and bison to build the Abstract Syntax Tree.
typedef union
{
  TAO_ETCL_Constraint* constraint;
} YYSTYPE;

extern YYSTYPE yylval;
extern YYSTYPE yyval;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ETCL_INTERPRETER_H */
