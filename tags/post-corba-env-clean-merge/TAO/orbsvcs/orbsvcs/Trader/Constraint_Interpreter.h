/* -*- C++ -*- */
// $Id$

// ========================================================================
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
#include "ace/pre.h"

#include "Constraint_Nodes.h"
#include "Constraint_Visitors.h"
#include "Interpreter.h"

#include "orbsvcs/CosTradingS.h"
#include "orbsvcs/CosTradingReposS.h"

class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;

class TAO_Trading_Export TAO_Constraint_Interpreter : public TAO_Interpreter
{
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
public:
  // = Initialization and termination methods.
  TAO_Constraint_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* constraints
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalConstraint,
                     CORBA::NO_MEMORY));

  TAO_Constraint_Interpreter (TAO_Constraint_Validator& validator,
                              const char* constraints
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalConstraint,
                     CORBA::NO_MEMORY));
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

class TAO_Trading_Export TAO_Preference_Interpreter : public TAO_Interpreter
{
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
public:
  // = Initialization and termination methods.
  TAO_Preference_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* preference
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Lookup::IllegalPreference,
                     CORBA::NO_MEMORY));

  TAO_Preference_Interpreter (TAO_Constraint_Validator& validator,
                              const char* preference
                              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Lookup::IllegalPreference,
                     CORBA::NO_MEMORY));

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

#include "ace/post.h"
#endif /* TAO_CONSTRAINT_INTERPRETER_H */
