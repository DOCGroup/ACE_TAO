// $Id$

// ============================================================================
//
// = FILENAME
//    QuoterFactoryFinder_Impl.cpp
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
#include "QuoterFactoryFinder_Impl.h"

static const char usage [] = 
"[-? |\n[-O[RBport] ORB port number]]";

// Constructor
QuoterFactoryFinder_i::QuoterFactoryFinder_i (void)
{
  TAO_TRY 
    {
      // Get a reference to the ORB.
      CORBA::ORB_var orb_var = TAO_ORB_Core_instance ()->orb ();
      TAO_CHECK_ENV;

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var = 
        orb_var->resolve_initial_references ("NameService");
      TAO_CHECK_ENV;

      if (CORBA::is_nil (namingObj_var.in ())) 
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1 
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      CORBA::Object_var quoterNamingObj_var = 
        namingContext_var->resolve (quoterContextName, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      quoterNamingContext_var_ = 
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           TAO_TRY_ENV);
       
      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.
      CosNaming::Name quoterFactoryFinderName_ (1);
      quoterFactoryFinderName_.length (1);
      quoterFactoryFinderName_[0].id = CORBA::string_dup ("QuoterFactoryFinder");

      this->quoterNamingContext_var_->bind (quoterFactoryFinderName_,
                                            (CORBA::Object *) this,
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;
}

// Destructor.
QuoterFactoryFinder_i::~QuoterFactoryFinder_i (void)
{
}


CosLifeCycle::Factories *
QuoterFactoryFinder_i::find_factories (const CosLifeCycle::Key &factory_key,
                                       CORBA::Environment &_env_there)
{
  CORBA::Environment env_here;

  // fill in the name of the Quoter Factory
  // CosNaming::Name factoryName (1);  // max = 1 
  // factoryName.length (1);
  // factoryName[0].id = CORBA::string_dup ("quoter_factory");
  // or
  CosNaming::Name factoryName = (CosNaming::Name) factory_key;

  // Try to get a reference to a Quoter Factory
  CORBA::Object_var quoterFactoryObject_var =  
    quoterNamingContext_var_->resolve (factoryName, env_here);
  
  // see if there is an exception, if yes then throw the NoFactory exception
  if (env_here.exception () != 0) {

    // throw a NoFactory exception  
    _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
    return 0;
  }

  // Check if it is a valid Quoter Factory reference
  if (CORBA::is_nil (quoterFactoryObject_var.in())) {

    // throw a NoFactory exception  
    _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
    return 0;
  }

  // were able to get a reference to Quoter Factory
  else { 

    // create a sequence of factories object
    CosLifeCycle::Factories *factories_ptr = new CosLifeCycle::Factories (1);      

    // See if there is an exception, if yes then throw the NoFactory
    // exception.
    if (env_here.exception () != 0) 
    {
      // Throw a NoFactory exception.
      _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

    // using the Naming Service only one reference is available
    factories_ptr->length (1);      

    // Check if it is a valid Quoter Factory reference.
    if (CORBA::is_nil (quoterFactoryObject_var.in ())) 
    {
      // throw a NoFactory exception.
      _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

    // insert the object reference 
    (*factories_ptr)[0] = quoterFactoryObject_var;

    ACE_DEBUG ((LM_DEBUG,"Have reference to a Quoter Factory.\n"));
    return factories_ptr;
  }
}
