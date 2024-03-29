
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
  Criteria_Evaluator (const CosLifeCycle::Criteria &criteria);
  ~Criteria_Evaluator ();

  const LifeCycleService::Criteria_Evaluator::SeqNamedValuePair * getInitialization ();

  char * getFilter ();

  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair * getLogicalLocation ()
    {
      return 0;
    }

  char * getPreferences ()
    {
      return 0;
    }

private:
  CORBA::Any *getCriteriaMember (const char *member_name);

  const CosLifeCycle::Criteria &criteria_;
};

#endif /* CRITERIA_EVALUATOR_H */





