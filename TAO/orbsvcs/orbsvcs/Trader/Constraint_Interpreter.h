/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Constraint_Interpreter.h
 *
 *  $Id$
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONSTRAINT_INTERPRETER_H
#define TAO_CONSTRAINT_INTERPRETER_H
#include /**/ "ace/pre.h"

#include "Constraint_Nodes.h"
#include "Constraint_Visitors.h"
#include "Interpreter.h"

#include "orbsvcs/CosTradingS.h"
#include "orbsvcs/CosTradingReposS.h"

class TAO_Constraint_Evaluator;
class TAO_Constraint_Validator;

/**
 * @class TAO_Constraint_Interpreter
 *
 * @brief TAO_Constraint_Interpreter will, given a constraint string whose
 * syntax and semantics comply with the trader specification for the
 * constraint language, determine if a CosTrading::Offer meets the
 * constraints.
 *
 * TAO_Constraint_Interpreter will first build an expression tree
 * representing the constraint expression using Lex and Yacc. Then,
 * using a TAO_Constraint_Validator, it will validate the semantic
 * correctness of the tree. When the evaluate method is invoked with
 * an Offer, the TAO_Constraint_Interpreter will construct an
 * EvaluationVisitor, which will evaluate the tree and decide
 * whether the offer meets the constraints.
 */
class TAO_Trading_Export TAO_Constraint_Interpreter : public TAO_Interpreter
{
public:
  // = Initialization and termination methods.
  TAO_Constraint_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* constraints
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalConstraint,
                     CORBA::NO_MEMORY));

  /**
   * This constructor builds an expression tree representing the
   * constraint specified in <constraints>, and throws an Illegal
   * Constraint exception if the constraint given has syntax errors or
   * semantic errors, such as mismatched types.
   */
  TAO_Constraint_Interpreter (TAO_Constraint_Validator& validator,
                              const char* constraints
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::IllegalConstraint,
                     CORBA::NO_MEMORY));

  /// Destructor
  ~TAO_Constraint_Interpreter (void);

  CORBA::Boolean evaluate (CosTrading::Offer* offer);

  CORBA::Boolean evaluate (TAO_Constraint_Evaluator& evaluator);

  // Determine whether an offer fits the constraints with which the
  // tree was constructed. This method is thread safe (hopefully).
};

/**
 * @class TAO_Preference_Interpreter
 *
 * @brief The TAO_Preference_Interpreter will, given a valid preference
 * string and offers, will order the offers based on the offers'
 * compliance with the preferences.
 *
 * Each time the order_offer method is invoked, the
 * TAO_Preference_Interpreter stores the offer reference in the
 * order dictated by its evaluation of the preference string. After
 * the TAO_Preference_Interpreter client has finished ordering all
 * the offers, it will extract the offers in order using the
 * remove_offer method.
 */
class TAO_Trading_Export TAO_Preference_Interpreter : public TAO_Interpreter
{
public:
  // = Initialization and termination methods.
  TAO_Preference_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                              const char* preference
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Lookup::IllegalPreference,
                     CORBA::NO_MEMORY));

  /**
	* Parse the preference string, determining first if it's
	* valid. Throw an IllegalPreference exception if the preference
	* doesn't conform to the BNF grammar for preferences.
	*/
  TAO_Preference_Interpreter (TAO_Constraint_Validator& validator,
                              const char* preference
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CosTrading::Lookup::IllegalPreference,
                     CORBA::NO_MEMORY));

  /// Destructor
  ~TAO_Preference_Interpreter(void);

  void order_offer (CosTrading::Offer* offer,
                    CosTrading::OfferId offer_id = 0);

  /// Evaluate the offer, and order it internally based on the results
  /// of the evaluation.
  void order_offer (TAO_Constraint_Evaluator& evaluator,
                    CosTrading::Offer* offer,
                    CosTrading::OfferId offer_id = 0);

  int remove_offer (CosTrading::Offer*& offer,
                    CosTrading::OfferId& offer_id);

  /// Remove the next offer. The offer returned will be the next in the
  /// ordering determined by the preference string.
  int remove_offer (CosTrading::Offer*& offer);

  /// Return the number of offers remaining in the ordering.
  size_t num_offers (void);

  struct Preference_Info
  {
    /// True if the preference evaluation didn't return an error for this offer.
    CORBA::Boolean evaluated_;

    /// The value of the preference evaluation.
    TAO_Literal_Constraint value_;

    /// The offer id of this offer.
    CosTrading::OfferId offer_id_;

    /// A pointer to the offer.
    CosTrading::Offer* offer_;
  };

  typedef ACE_Unbounded_Queue<Preference_Info> Ordered_Offers;

private:

  /// Disallow copying.
  TAO_Preference_Interpreter (const TAO_Preference_Interpreter&);
  TAO_Preference_Interpreter& operator= (const TAO_Preference_Interpreter&);

  /// The ordered list of offers.
  Ordered_Offers offers_;
};

#include /**/ "ace/post.h"
#endif /* TAO_CONSTRAINT_INTERPRETER_H */
