
//=============================================================================
/**
 *  @file    Criteria_Evaluator.h
 *
 *  An Evaluator for the CosLifeCycle Criteria
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================

#ifndef CRITERIA_EVALUATOR_H_H
#define CRITERIA_EVALUATOR_H_H

#include "orbsvcs/CosLifeCycleC.h"
#include "orbsvcs/LifeCycleServiceC.h"

#include <string>

/**
 * @class Criteria_Evaluator
 */
class Criteria_Evaluator// : public LifeCycleService::Criteria_Evaluator
{
public:
  explicit Criteria_Evaluator (CosLifeCycle::Criteria const & criteria);
  Criteria_Evaluator (Criteria_Evaluator const &) = delete;
  Criteria_Evaluator (Criteria_Evaluator &&) = delete;
  Criteria_Evaluator &operator= (Criteria_Evaluator const &) = delete;
  Criteria_Evaluator &operator= (Criteria_Evaluator &&) = delete;

  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const * getInitialization () const;

  std::string getFilter () const;

  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair * getLogicalLocation () const
    {
      return {};
    }

  char * getPreferences () const
    {
      return {};
    }

private:
  CORBA::Any const * getCriteriaMember (char const * member_name) const;

  CosLifeCycle::Criteria const & criteria_;
};

#endif /* CRITERIA_EVALUATOR_H */

