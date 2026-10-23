
//=============================================================================
/**
 *  @file    Criteria_Evaluator.h
 *
 *  An Evaluator for the CosLifeCycle Criteria
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================

#include "orbsvcs/CosLifeCycleC.h"
#include "orbsvcs/LifeCycleServiceC.h"

#ifndef CRITERIA_EVALUATOR_H_H
#define CRITERIA_EVALUATOR_H_H

/**
 * @class Criteria_Evaluator
 */
class Criteria_Evaluator// : public LifeCycleService::Criteria_Evaluator
{
public:
  explicit Criteria_Evaluator (CosLifeCycle::Criteria const & criteria);
  ~Criteria_Evaluator ();

  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const * getInitialization ();

  char * getFilter ();

  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair * getLogicalLocation ()
    {
      return nullptr;
    }

  char * getPreferences ()
    {
      return nullptr;
    }

private:
  CORBA::Any * getCriteriaMember (char const * member_name);

  CosLifeCycle::Criteria const & criteria_;
};

#endif /* CRITERIA_EVALUATOR_H */





