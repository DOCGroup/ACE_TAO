// $Id$

// ============================================================================
//
// = FILENAME
//    Criteria_Evaluator.cpp
//
// = DESCRIPTION
//    An Evaluator for the CosLifeCycle Criteria
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "Criteria_Evaluator.h"

ACE_RCSID(LifeCycle_Service, Criteria_Evaluator, "$Id$")

Criteria_Evaluator::Criteria_Evaluator (const CosLifeCycle::Criteria &criteria)
: criteria_ (criteria)
{
}

Criteria_Evaluator::~Criteria_Evaluator ()
{
}



LifeCycleService::Criteria_Evaluator::SeqNamedValuePair *
Criteria_Evaluator::getInitialization (CORBA::Environment &env)
{
  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair_ptr sequence_ptr = 0;

  CORBA::Any * any_ptr = this->getCriteriaMember ("initialization");

  if (any_ptr == 0)
  {
    env.exception (new LifeCycleService::Criteria_Evaluator::NotAvailable ("No initialization member found.\n"));
    return 0;
  }

  *any_ptr >>= sequence_ptr;  

  return sequence_ptr;
}


char *
Criteria_Evaluator::getFilter (CORBA::Environment &env)
{
  CORBA::String string;
  CORBA::Any value;
  CORBA::Any_ptr any_ptr = this->getCriteriaMember ("filter");

  if (any_ptr == 0)
  {
    env.exception (new LifeCycleService::Criteria_Evaluator::NotAvailable ("No filter member found.\n"));
    return 0;
  }
  *any_ptr >>= string;
  return string;
}


CORBA::Any *
Criteria_Evaluator::getCriteriaMember (const CORBA::String member_name)
{
  if (!ACE_OS::strcmp(member_name, "initialization")
    && !ACE_OS::strcmp(member_name, "filter")
    && !ACE_OS::strcmp(member_name, "logical location")
    && !ACE_OS::strcmp(member_name, "preferences"))
    return 0;

  for (unsigned int i = 0; i < criteria_.length(); i++)
  {
    if (ACE_OS::strcmp (member_name, criteria_[i].name) == 0)
    {
      CORBA::Any_ptr value_ptr;
      ACE_NEW_RETURN (value_ptr,
                      CORBA::Any(criteria_[i].value),
                      0);
      return value_ptr;
    }
  }
  return 0;
}

