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

// default constructor
Criteria_Evaluator::NotAvailable::NotAvailable (void)
  : CORBA_UserException (0) // no type code available
{
}

Criteria_Evaluator::NotAvailable::NotAvailable(const char * reason)
  : CORBA_UserException  (0) // no type code available
{
    this->reason_ = CORBA::string_dup (reason);
}

// copy constructor
Criteria_Evaluator::NotAvailable::NotAvailable (const NotAvailable &_tao_excp)
  :CORBA_UserException (0)
{
    this->reason_ = CORBA::string_dup (_tao_excp.reason_.in ());
}

Criteria_Evaluator::NotAvailable::~NotAvailable (void)
{
}

// assignment operator
Criteria_Evaluator::NotAvailable&
Criteria_Evaluator::NotAvailable::operator= (const NotAvailable &_tao_excp)
{
  this->type_ = 0;
  this->reason_ = CORBA::string_dup (_tao_excp.reason_.in ());
  return *this;
}


Criteria_Evaluator::Criteria_Evaluator (CosLifeCycle::Criteria &criteria)
: criteria_ (criteria)
{
}
 
Criteria_Evaluator::~Criteria_Evaluator ()
{
}


  
TAO_Unbounded_Object_Sequence<CosLifeCycle::NVP> *
Criteria_Evaluator::getInitialization (CORBA::Environment &env)
{
  TAO_Unbounded_Object_Sequence<CosLifeCycle::NVP> *sequence_ptr;

  CORBA::Any_ptr any_ptr = getCriteriaMember ("initialization");

  if (any_ptr == 0)
  {
    env.exception (new NotAvailable ("No initialization member found.\n"));      
    return 0;
  }
  //*any_ptr >>= sequence_ptr;

  return sequence_ptr;
}
  
  
CORBA::String
Criteria_Evaluator::getFilter (CORBA::Environment &env)
{
  CORBA::String string;
  CORBA::Any_ptr any_ptr = getCriteriaMember ("filter");

  if (any_ptr == 0)
  {
    env.exception (new NotAvailable ("No filter member found.\n"));      
    return 0;
  }
  *any_ptr >>= string;
  return string;
}


CORBA::Any_ptr
Criteria_Evaluator::getCriteriaMember (const CORBA::String member_name)
{
  if (!ACE_OS::strcmp(member_name, "initialization") 
    && !ACE_OS::strcmp(member_name, "filter")
    && !ACE_OS::strcmp(member_name, "logical location")
    && !ACE_OS::strcmp(member_name, "preferences"))
    return 0;

  for (unsigned int i = 0; i < criteria_.length(); i++)
  {
    if (ACE_OS::strcmp (member_name, criteria_[i].name))
    {
      return &(criteria_[i].value);
    }
  }
  return 0;  
}

