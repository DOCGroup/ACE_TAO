
//=============================================================================
/**
 *  @file    Trader_Constraint_Visitors.h
 *
 *  $Id$
 *
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TRADER_CONSTRAINT_VISITOR_H
#define TAO_TRADER_CONSTRAINT_VISITOR_H
#include "ace/pre.h"

#include "Constraint_Visitors.h"
#include "Trader.h"
#include "Trader_Utils.h"
#include "orbsvcs/CosTradingC.h"

class TAO_Trading_Export TAO_Trader_Constraint_Validator : public TAO_Constraint_Validator
{
 public:
  /**
   * The constructor creates a map of property names to their values
   * from the Type Description retrieved from the
   * ServiceTypeRepository. The ServiceTypeRepository throws
   * exceptions when it's displeased with the type name provided to
   * it. The map allows O(lg n) associative access, rather than the
   * O(n) sequential lookup from the CORBA data structures.
   */
  TAO_Trader_Constraint_Validator (const CosTradingRepos::ServiceTypeRepository::TypeStruct&
                                   type_struct);

  /// Desctructor.
  virtual ~TAO_Trader_Constraint_Validator (void);

};

class TAO_Trading_Export TAO_Trader_Constraint_Evaluator : public TAO_Constraint_Evaluator
{
 public:
  /// Constructor.
  TAO_Trader_Constraint_Evaluator (CosTrading::Offer* offer,
                                   CORBA::Boolean supports_dynamic_properties = 1);

  virtual int visit_property (TAO_Property_Constraint* literal);

 private:
  /// Utility with which to evaluate the properties of an offer, be
  /// they dyanmic or static.
  TAO_Property_Evaluator prop_eval_;
};

#include "ace/post.h"
#endif /* TAO_TRADER_CONSTRAINT_VISITOR_H */
