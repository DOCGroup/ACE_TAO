// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service_Impl.cpp
//
// = DESCRIPTION
//    The implementation of a LifeCycle Service GenericFactory for the 
//    Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "Life_Cycle_Service_Impl.h"
#include "QuoterC.h"

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

// Constructor
Quoter_Life_Cycle_Service_Impl::Quoter_Life_Cycle_Service_Impl (void)
{
}

// Destructor.
Quoter_Life_Cycle_Service_Impl::~Quoter_Life_Cycle_Service_Impl (void)
{
}


CORBA::Boolean 
Quoter_Life_Cycle_Service_Impl::supports (const CosLifeCycle::Key &factory_key,
                                       CORBA::Environment &_env_there)
{
  ACE_UNUSED_ARG (factory_key);
  ACE_UNUSED_ARG (_env_there);
  return 0;
}

CORBA::Object_ptr
Quoter_Life_Cycle_Service_Impl::create_object (const CosLifeCycle::Key &factory_key, 
                                            const CosLifeCycle::Criteria &the_criteria,
                                            CORBA::Environment &_env_there)
{
  // Exceptions are forwarded, not handled !!

  if (factory_trader_ptr_ != 0)
    {
      if (criteria_evaluator_var_ == 0)
	{
	  ACE_NEW_RETURN (criteria_evaluator_var_, Quoter_Criteria_Evaluator(the_criteria), 0);
	}
      else // the object is instantiated already
	{
	  CORBA::Environment env;

	  // feed in the criteria
	  criteria_evaluator_var_->setCriteria (the_criteria, _env_there);

	  if (_env_there.exception() != 0)
	    {
	      return 0;
	    } 
	}
      
      if (criteria_evaluator_var_ != 0)
	{	  
	  CORBA::String filter = criteria_evaluator_var_->getFilter (_env_there);
	  
	  if (_env_there.exception() != 0)
	    {
	      return 0;
	    } 
	  
	  CORBA::Object_ptr object_ptr = factory_trader_ptr_->query (filter);
	  	  
	  if (object_ptr != 0)
	    ACE_DEBUG ((LM_DEBUG, "Factory reference OK.\n"));    

	  return CORBA::Object::_duplicate (object_ptr);
	}
      else // pointer is wrong
	{
	  return 0;
	}
    }
  else
    {
      return 0;
    }
}

void 
Quoter_Life_Cycle_Service_Impl::register_factory (const char * name,
					       const char * location,
					       const char * description,
					       CORBA::Object_ptr object,
					       CORBA::Environment &_env_there)
{
  if (factory_trader_ptr_ == 0)
    {
      ACE_NEW (factory_trader_ptr_, Factory_Trader());
    }
  // we have an proper reference to the Factory Trader
  
  // Just forward the call to the factory trader 
  // Exceptions are not handled here, they are returned
  factory_trader_ptr_->export (name, location, description, object);     

  ACE_DEBUG ((LM_DEBUG, "Registered a factory with:\n"
	      "     name: %s\n"
	      "     location: %s\n"
	      "     description: %s\n"
	      name, location, description));       
}


 


