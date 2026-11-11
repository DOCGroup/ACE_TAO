
//=============================================================================
/**
 *  @file    Criteria_Evaluator.cpp
 *
 *  An Evaluator for the CosLifeCycle Criteria
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "Criteria_Evaluator.h"

#include <cstring>
#include <memory>

Criteria_Evaluator::Criteria_Evaluator (CosLifeCycle::Criteria const &criteria)
  : criteria_ (criteria)
{
}

LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const *
Criteria_Evaluator::getInitialization () const
{
  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const * sequence_ptr {};

  CORBA::Any const * const any_ptr = this->getCriteriaMember ("initialization");

  if (any_ptr == nullptr)
    throw LifeCycleService::Criteria_Evaluator::NotAvailable (
      "No initialization member found.\n");

  *any_ptr >>= sequence_ptr;

  return sequence_ptr;
}

std::string
Criteria_Evaluator::getFilter () const
{
  char const * string {};
  CORBA::Any const * const any_ptr = this->getCriteriaMember ("filter");

  if (any_ptr == nullptr)
    throw LifeCycleService::Criteria_Evaluator::NotAvailable (
      "No filter member found.\n");
  if (!(*any_ptr >>= string) || string == nullptr)
    throw LifeCycleService::Criteria_Evaluator::NotAvailable (
      "The filter member is not a string.\n");
  return string;
}

CORBA::Any const *
Criteria_Evaluator::getCriteriaMember (char const *member_name) const
{
  for (CORBA::ULong i {}; i < this->criteria_.length (); ++i)
    if (std::strcmp (member_name, this->criteria_[i].name) == 0)
      return std::addressof (this->criteria_[i].value);

  return nullptr;
}
