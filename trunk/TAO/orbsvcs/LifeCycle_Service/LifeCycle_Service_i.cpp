// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service_i.cpp
//
// = DESCRIPTION
//    The implementation of a LifeCycle Service GenericFactory.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "orbsvcs/LifeCycleServiceC.h"
#include "LifeCycle_Service_i.h"

ACE_RCSID(LifeCycle_Service, LifeCycle_Service_i, "$Id$")

// Constructor
Life_Cycle_Service_i::Life_Cycle_Service_i (void)
  : factory_trader_ptr_ (0)
{
}

// Destructor.
Life_Cycle_Service_i::~Life_Cycle_Service_i (void)
{
}


CORBA::Boolean
Life_Cycle_Service_i::supports (const CosLifeCycle::Key &factory_key,
                                       CORBA::Environment &TAO_IN_ENV_there)
{
  ACE_UNUSED_ARG (factory_key);
  ACE_UNUSED_ARG (TAO_IN_ENV_there);
  return 0;
}

CORBA::Object_ptr
Life_Cycle_Service_i::create_object (const CosLifeCycle::Key &factory_key,
                                            const CosLifeCycle::Criteria &the_criteria,
                                            CORBA::Environment &TAO_IN_ENV_there)
{
  ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i:create_object: called.\n"));
  // Exceptions are forwarded, not handled !!

  if (factory_trader_ptr_ != 0)
    {
      Criteria_Evaluator criteria_Evaluator(the_criteria);
      ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i:create_object: new evaluator.\n"));

      ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i:create_object: getFilter will be called.\n"));

      CORBA::String filter = criteria_Evaluator.getFilter (TAO_IN_ENV_there);

      if (TAO_IN_ENV_there.exception() != 0)
	{
	  return 0;
	}

      ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i:create_object: query(%s) will be called.\n",filter));

      CORBA::Object_ptr genericFactoryObj_ptr = factory_trader_ptr_->query (filter);

      ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i:create_object: query was called.\n"));

      if (CORBA::is_nil (genericFactoryObj_ptr))
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Life_Cycle_Service_i::create_object: Factory is nil!\n"),
			  0);
      else // everyting is ok
	ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i::create_object: Object reference OK.\n"));

      // Now we have a proper reference to a Generic Factory
      // the create_object call will be forwarded to this factory


      // Check if it is a valid Generic Factory reference
      if (CORBA::is_nil (genericFactoryObj_ptr))
	{ // throw a NoFactory exception
	  TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));
	  return 0;
	}
      else
	{
	  CORBA::Environment env_here;

	  CosLifeCycle::GenericFactory_var genericFactory_var =
	    CosLifeCycle::GenericFactory::_narrow (genericFactoryObj_ptr,
					    env_here);

	  // see if there is an exception, if yes then throw the NoFactory exception
	  if (env_here.exception () != 0) // throw a NoFactory exception
	    {
	      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));
	      return 0;
	    }

	  if (CORBA::is_nil (genericFactory_var.in()))
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "Life_Cycle_Service_i::create_object: Invalid Generic Factory.\n"),
			      0);

	  ACE_DEBUG ((LM_DEBUG, "Life_Cycle_Service_i::create_object: Generic Factory reference OK.\n"));

	  // Now retrieve the Object obj ref corresponding to the key.
	  CORBA::Object_var object_var = genericFactory_var->create_object (factory_key,
									    the_criteria,
									    TAO_IN_ENV_there);

	  ACE_DEBUG ((LM_DEBUG,
		      "Life_Cycle_Service_i::create_object: Forwarded request.\n"));

	  if (CORBA::is_nil (object_var.in()))
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "Life_Cycle_Service_i::create_object: Null object refeference returned by factory.\n"),
			      0);

	  ACE_DEBUG ((LM_DEBUG,
		      "Life_Cycle_Service_i::create_object: Return a object reference to a new object.\n"));

	  return CORBA::Object::_duplicate (object_var.in());
	}
    }
  else
    {
      return 0;
    }
}

void
Life_Cycle_Service_i::register_factory (const char * name,
					   const char * location,
					   const char * description,
					   CORBA::Object_ptr object,
					   CORBA::Environment &TAO_IN_ENV_there)
{
  if (factory_trader_ptr_ == 0)
    {
      ACE_NEW (factory_trader_ptr_, Factory_Trader());
    }
  // we have an proper reference to the Factory Trader

  // Just forward the call to the factory trader
  // Exceptions are not handled here, they are returned
  factory_trader_ptr_->_cxx_export (name, location, description, object);

  ACE_DEBUG ((LM_DEBUG, "Registered a factory with:\n"
	      "     name: %s\n"
	      "     location: %s\n"
	      "     description: %s\n",
	      name, location, description));
}
