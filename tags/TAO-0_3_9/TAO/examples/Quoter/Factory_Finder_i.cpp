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
  // Nothing
}

// Destructor.
Quoter_Factory_Finder_i::~Quoter_Factory_Finder_i (void)
{
  // Nothing
}


CosLifeCycle::Factories *
Quoter_Factory_Finder_i::find_factories (const CosLifeCycle::Key &factory_key,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  const char *exception_message = "Null Message";

  CosLifeCycle::Factories *factories_ptr = 0;
  
  ACE_TRY
  {
    // Get a reference to the ORB.
    CORBA::ORB_ptr orb_ptr = TAO_ORB_Core_instance ()->orb ();

    // Get the Naming Service object reference.
    exception_message = "While resolving the Name Service";
    CORBA::Object_var namingObj_var =
      orb_ptr->resolve_initial_references ("NameService", ACE_TRY_ENV);
    ACE_TRY_CHECK;

    if (CORBA::is_nil (namingObj_var.in ()))
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable get the Naming Service.\n"));

    // Narrow the object reference to a Naming Context.
    exception_message = "While narrowing the Naming Context";
    CosNaming::NamingContext_var namingContext_var =
      CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                         ACE_TRY_ENV);
    ACE_TRY_CHECK;

    // Take the key supplied to search for a Quoter Factory
    CosNaming::Name factoryName = (CosNaming::Name) factory_key;

    // Try to get a reference to a Quoter Factory
    exception_message = "While resolving the Factory Object";
    CORBA::Object_var quoterFactoryObject_var =
      namingContext_var->resolve (factoryName, ACE_TRY_ENV);
    ACE_TRY_CHECK;

    // Check if it is a valid Quoter Factory reference
    if (CORBA::is_nil (quoterFactoryObject_var.in()))
      ACE_THROW_RETURN (CosLifeCycle::NoFactory (factory_key), 0);

    // create a sequence of factories object
    factories_ptr = new CosLifeCycle::Factories (1);

    // using the Naming Service only one reference is available
    factories_ptr->length (1);

    // Check if it is a valid Quoter Factory reference.
    if (CORBA::is_nil (quoterFactoryObject_var.in ())) // throw a NoFactory exception.
      ACE_THROW_RETURN (CosLifeCycle::NoFactory (factory_key), 0);

    // insert the object reference
    (*factories_ptr)[0] = CORBA::Object::_duplicate (quoterFactoryObject_var.ptr());

    ACE_DEBUG ((LM_DEBUG,
                "Have reference to a Quoter Factory.\n"));
  }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Factory_Finder::find_factories - %s\n", exception_message));
      ACE_THROW_RETURN (CosLifeCycle::NoFactory (factory_key), 0);
    }
  ACE_ENDTRY;

  return factories_ptr;
}
