// $Id$

// ============================================================================
//
// = FILENAME
//    FactoryFinder_i.cpp
//
// = DESCRIPTION
//    A Factory Finder for the Quoter example. This example conforms
//    to the CosLifeCycle Factory Finder notion.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "Factory_Finder_i.h"
#include "QuoterC.h"

ACE_RCSID(Quoter, Factory_Finder_i, "$Id$")

// Constructor
Quoter_Factory_Finder_i::Quoter_Factory_Finder_i (void)
{
}

// Destructor.
Quoter_Factory_Finder_i::~Quoter_Factory_Finder_i (void)
{
}


CosLifeCycle::Factories *
Quoter_Factory_Finder_i::find_factories (const CosLifeCycle::Key &factory_key,
                                       CORBA::Environment &TAO_IN_ENV_there)
{
  CORBA::Environment env_here;

  // Get a reference to the ORB.
  CORBA::ORB_ptr orb_ptr = TAO_ORB_Core_instance ()->orb ();

  // Get the Naming Service object reference.
  CORBA::Object_var namingObj_var = 
    orb_ptr->resolve_initial_references ("NameService");

  if (CORBA::is_nil (namingObj_var.in ())) 
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable get the Naming Service.\n"));

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


  // Take the key supplied to search for a Quoter Factory
  CosNaming::Name factoryName = (CosNaming::Name) factory_key;

  // Try to get a reference to a Quoter Factory
  CORBA::Object_var quoterFactoryObject_var =  
    namingContext_var->resolve (factoryName, env_here);
  
  // see if there is an exception, if yes then throw the NoFactory exception
  if (env_here.exception () != 0) // throw a NoFactory exception  
    { 
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  // were able to get a reference to Quoter Factory

  // Check if it is a valid Quoter Factory reference
  if (CORBA::is_nil (quoterFactoryObject_var.in())) 
    { // throw a NoFactory exception  
      TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }
  else 
    {

      // create a sequence of factories object
      CosLifeCycle::Factories *factories_ptr = new CosLifeCycle::Factories (1);      

      // See if there is an exception, if yes then throw the NoFactory
      // exception.
      if (env_here.exception () != 0) 
        {
          // Throw a NoFactory exception.
          TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
          return 0;
        }

      // using the Naming Service only one reference is available
      factories_ptr->length (1);      

      // Check if it is a valid Quoter Factory reference.
      if (CORBA::is_nil (quoterFactoryObject_var.in ())) // throw a NoFactory exception.
        {      
          TAO_IN_ENV_there.exception (new CosLifeCycle::NoFactory (factory_key));      
          return 0;
        }

      // insert the object reference 
      (*factories_ptr)[0] = CORBA::Object::_duplicate (quoterFactoryObject_var.ptr());

      ACE_DEBUG ((LM_DEBUG,
                  "Have reference to a Quoter Factory.\n"));
      return factories_ptr;
    }
}
