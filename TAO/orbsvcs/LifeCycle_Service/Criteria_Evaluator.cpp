
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
#include "ace/OS_NS_string.h"


Criteria_Evaluator::Criteria_Evaluator (CosLifeCycle::Criteria const & criteria)
: criteria_ (criteria)
{
}

Criteria_Evaluator::~Criteria_Evaluator () = default;

LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const *
Criteria_Evaluator::getInitialization ()
{
  LifeCycleService::Criteria_Evaluator::SeqNamedValuePair const * sequence_ptr {};

  CORBA::Any_ptr any_ptr =
    this->getCriteriaMember ("initialization");

  if (any_ptr == nullptr)
    throw LifeCycleService::Criteria_Evaluator::NotAvailable(
      "No initialization member found.\n");

  *any_ptr >>= sequence_ptr;

  return sequence_ptr;
}

char *
Criteria_Evaluator::getFilter ()
{
  char const * string {};
  CORBA::Any_ptr any_ptr = this->getCriteriaMember ("filter");

  if (any_ptr == nullptr)
    throw LifeCycleService::Criteria_Evaluator::NotAvailable(
      "No filter member found.\n");
  *any_ptr >>= string;
  return CORBA::string_dup (string);
}

CORBA::Any *
Criteria_Evaluator::getCriteriaMember (char const * member_name)
{
  if (!ACE_OS::strcmp(member_name, "initialization")
      && !ACE_OS::strcmp(member_name, "filter")
      && !ACE_OS::strcmp(member_name, "logical location")
      && !ACE_OS::strcmp(member_name, "preferences"))
    return nullptr;

  for (u_int i = 0;
       i < criteria_.length();
       i++)
    if (ACE_OS::strcmp (member_name,
                        criteria_[i].name) == 0)
      {
        CORBA::Any_ptr value_ptr {};

        // @@ We should use ACE_NEW_THROW_EX + ACE_CHECK_RETURN here.
        ACE_NEW_RETURN (value_ptr,
                        CORBA::Any(criteria_[i].value),
                        0);
        return value_ptr;
      }

  return nullptr;
}
