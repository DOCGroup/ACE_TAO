// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory_i.cpp
//
// = DESCRIPTION
//    The implementation of a LifeCycle Service GenericFactory for the 
//    Quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "Generic_Factory_i.h"
#include "QuoterC.h"

ACE_RCSID(Quoter, Generic_Factory_i, "$Id$")

// Constructor
Quoter_Generic_Factory_i::Quoter_Generic_Factory_i (void)
{
}

// Destructor.
Quoter_Generic_Factory_i::~Quoter_Generic_Factory_i (void)
{
}

CORBA::Boolean 
Quoter_Generic_Factory_i::supports (const CosLifeCycle::Key &factory_key,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (factory_key);
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  return 0;
}



CosNaming::NamingContext_ptr
Quoter_Generic_Factory_i::get_naming_context (const CosLifeCycle::Key &factory_key,
                                              CORBA::Environment &env_here,
                                              CORBA::Environment &TAO_IN_ENV_there)
{
  // Get a reference to the ORB.
  CORBA::ORB_ptr orb_ptr =
    TAO_ORB_Core_instance ()->orb ();

  // Get the Naming Service object reference.
  CORBA::Object_var namingObj_var = 
    orb_ptr->resolve_initial_references ("NameService");

  if (CORBA::is_nil (namingObj_var.in ())) 
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) Unable get the Naming Service.\n"));

  // Narrow the object reference to a Naming Context.
  CosNaming::NamingContext_var namingContext_var =
    CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                       env_here);

  // See if there is an exception, if yes then throw the NoFactory
  // exception.
  if (env_here.exception () != 0) // throw a NoFactory exception  
    { 
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  // Get the IDL_Quoter naming context.
  CosNaming::Name quoterContextName (1);  // max = 1 
  quoterContextName.length (1);
  quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

  CORBA::Object_var quoterNamingObj_var = 
    namingContext_var->resolve (quoterContextName, env_here);

  // See if there is an exception, if yes then throw the NoFactory
  // exception.
  if (env_here.exception () != 0) // throw a NoFactory exception  
    { 
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  CosNaming::NamingContext_var quoterNamingContext_var = 
    CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                       env_here);
  // See if there is an exception, if yes then throw the NoFactory
  // exception.
  if (env_here.exception () != 0) // throw a NoFactory exception  
    { 
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }


  return CosNaming::NamingContext::_duplicate (quoterNamingContext_var.in ());
}


  
CORBA::Object_ptr
Quoter_Generic_Factory_i::create_object (const CosLifeCycle::Key &factory_key, 
                                            const CosLifeCycle::Criteria &the_criteria,
                                            CORBA::Environment &TAO_IN_ENV_there)
{
  ACE_UNUSED_ARG (the_criteria);

  CORBA::Environment env_here;

  CosNaming::NamingContext_var quoterNamingContext_var = 
    this->get_naming_context (factory_key,
                              env_here, 
                              TAO_IN_ENV_there);

  // ** now a proper reference to the quoter naming context is
  // available

  // Fill in the name of the Quoter Factory.
  // Take the key supplied to search for a Quoter Factory
  CosNaming::Name factory_Name = (CosNaming::Name) factory_key;  

  // Try to get a reference to a Quoter Factory
  CORBA::Object_var quoterFactoryObject_var =  
    quoterNamingContext_var->resolve (factory_Name, env_here);
  
  // See if there is an exception, if yes then throw the NoFactory
  // exception.
  if (env_here.exception () != 0) // throw a NoFactory exception  
    { 
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  // We were able to get a reference to Quoter Factory.

  // Check if it is a valid Quoter Factory reference
  if (CORBA::is_nil (quoterFactoryObject_var.in())) 
    { // throw a NoFactory exception  
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }
  else 
    { 
      // Check if it is a valid Quoter Factory reference.
      if (CORBA::is_nil (quoterFactoryObject_var.in ())) // throw a NoFactory exception.
        {      
          TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
          return 0;
        }

      Stock::Quoter_Factory_var factory_var = 
        Stock::Quoter_Factory::_narrow (quoterFactoryObject_var.in (), 
                                        env_here);

      // See if there is an exception, if yes then throw the NoFactory
      // exception.
      if (env_here.exception () != 0) // throw a NoFactory exception  
        { 
          TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
          return 0;
        }
    
      if (CORBA::is_nil (factory_var.in ()))      
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid factory.\n"),
                          0);
                       
      ACE_DEBUG ((LM_DEBUG,
                  "Factory reference OK.\n"));    
      
      // Now retrieve the Quoter obj ref corresponding to the key.
      Stock::Quoter_var quoter_var =
        factory_var->create_quoter ("test",
                                    TAO_IN_ENV_there);
      ACE_DEBUG ((LM_DEBUG,
                  "Quoter Created\n"));
    
      if (CORBA::is_nil (quoter_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null quoter objref returned by factory\n"),
                          0);

      ACE_DEBUG ((LM_DEBUG,
                  "Return a object reference to a new object.\n"));

      CORBA::Object_ptr object_ptr =
        CORBA::Object::_duplicate ((CORBA::Object_ptr) quoter_var.ptr ());

      return object_ptr;
    }
} 
