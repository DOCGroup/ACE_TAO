/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Constraint_Interpreter.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_CONSTRAINT_INTERPRETER_H
#define TAO_CONSTRAINT_INTERPRETER_H

#include "Constraint_Nodes.h"
#include "Constraint_Visitors.h"

class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;

  // *************************************************************
  // TAO_Interpreter
  // *************************************************************

class TAO_Interpreter
// = TITLE
//      TAO_Interpreter is the superclass for all interpreters. Its
//      build tree method invokes the yacc parser to parse a constraint
//      or preference string.
{
protected:

  TAO_Interpreter (void) : root_ (0) {}

  ~TAO_Interpreter (void);

  int build_tree (const char* preferences);
  // Using the Yacc generated parser, construct an expression
  // tree representing <constraints> from the tokens returned by it.

  static int is_empty_string (const char* str);

  TAO_Constraint* root_;
  // The root of the expression tree, not equal to null if build_tree
  // successfully builds a tree from the constraints.
};

  // *************************************************************
  // TAO_Constraint_Interpreter
  // *************************************************************

class TAO_Constraint_Interpreter : public TAO_Interpreter
//
// = TITLE
//   TAO_Constraint_Interpreter will, given a constraint string whose
//   syntax and semantics comply with the trader specification for the
//   constraint language, determine if a CosTrading::Offer meets the
//   constraints.
//
// = DESCRIPTION
//   TAO_Constraint_Interpreter will first build an expression tree
//   representing the constraint expression using Lex and Yacc. Then,
//   using a TAO_Constraint_Validator, it will validate the semantic
//   correctness of the tree. When the evaluate method is invoked with
//   an Offer, the TAO_Constraint_Interpreter will construct an
//   EvaluationVisitor, which will evaluate the tree and decide
//   whether the offer meets the constraints.
{
public:

  TAO_Constraint_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* constraints,
                              CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::IllegalConstraint));

  TAO_Constraint_Interpreter (TAO_Constraint_Validator& validator,
                              const char* constraints,
                              CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::IllegalConstraint));
  // This constructor builds an expression tree representing the
  // constraint specified in <constraints>, and throws an Illegal
  // Constraint exception if the constraint given has syntax errors or
  // semantic errors, such as mismatched types.

  ~TAO_Constraint_Interpreter (void);
  // Destructor

  CORBA::Boolean evaluate (CosTrading::Offer* offer);

  CORBA::Boolean evaluate (TAO_Constraint_Evaluator& evaluator);

  // Determine whether an offer fits the constraints with which the
  // tree was constructed. This method is thread safe (hopefully).
};

  // *************************************************************
  // TAO_Preference_Interpreter
  // *************************************************************

class TAO_Preference_Interpreter : public TAO_Interpreter
// = TITLE
//   The TAO_Preference_Interpreter will, given a valid preference
//   string and offers, will order the offers based on the offers'
//   compliance with the preferences.
//
// = DESCRIPTION
//   Each time the order_offer method is invoked, the
//   TAO_Preference_Interpreter stores the offer reference in the
//   order dictated by its evaluation of the preference string. After
//   the TAO_Preference_Interpreter client has finished ordering all
//   the offers, it will extract the offers in order using the
//   remove_offer method.
{
public:

  TAO_Preference_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* preference,
                              CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference));

  TAO_Preference_Interpreter (TAO_Constraint_Validator& validator,
                              const char* preference,
                              CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference));

  // Parse the preference string, determining first if it's
  // valid. Throw an IllegalPreference exception if the preference
  // doesn't conform to the BNF grammar for preferences.

  ~TAO_Preference_Interpreter(void);
  // Destructor

  void order_offer (CosTrading::Offer* offer,
                    CosTrading::OfferId offer_id = 0);

  void order_offer (TAO_Constraint_Evaluator& evaluator,
                    CosTrading::Offer* offer,
                    CosTrading::OfferId offer_id = 0);
  // Evaluate the offer, and order it internally based on the results
  // of the evaluation.

  int remove_offer (CosTrading::Offer*& offer,
                    CosTrading::OfferId& offer_id);

  int remove_offer (CosTrading::Offer*& offer);
  // Remove the next offer. The offer returned will be the next in the
  // ordering determined by the preference string.

  int num_offers (void);
  // Return the number of offers remaining in the ordering.

  struct Preference_Info
  {
    CORBA::Boolean evaluated_;
    // True if the preference evaluation didn't return an error for this offer.

    TAO_Literal_Constraint value_;
    // The value of the preference evaluation.

    CosTrading::OfferId offer_id_;
    // The offer id of this offer.

    CosTrading::Offer* offer_;
    // A pointer to the offer.
  };

  typedef ACE_Unbounded_Queue<Preference_Info> Ordered_Offers;

private:

  TAO_Preference_Interpreter (const TAO_Preference_Interpreter&);
  TAO_Preference_Interpreter& operator= (const TAO_Preference_Interpreter&);
  // Disallow copying.

  Ordered_Offers offers_;
  // The ordered list of offers.
};

  // *************************************************************
  // Ugly Lex/Yacc Stuff
  // *************************************************************

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

#endif /* TAO_CONSTRAINT_INTERPRETER_H */
