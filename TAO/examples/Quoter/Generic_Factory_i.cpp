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
Quoter_Generic_Factory_i::supports (const CosLifeCycle::Key &,
                                    CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}



CosNaming::NamingContext_ptr
Quoter_Generic_Factory_i::get_naming_context (const CosLifeCycle::Key &factory_key,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  // Get a reference to the ORB.
  CORBA::ORB_ptr orb_ptr =
    TAO_ORB_Core_instance ()->orb ();

  ACE_TRY
    {
      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var =
        orb_ptr->resolve_initial_references ("NameService", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      // Get the IDL_Quoter naming context.
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var quoterNamingContext_var =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return quoterNamingContext_var._retn ();
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (CosLifeCycle::NoFactory (factory_key),
                        CosNaming::NamingContext::_nil ());
    }
  ACE_ENDTRY;
  return CosNaming::NamingContext::_nil ();
}



CORBA::Object_ptr
Quoter_Generic_Factory_i::create_object (const CosLifeCycle::Key &factory_key,
                                         const CosLifeCycle::Criteria &,
                                         CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria))
{
  ACE_TRY
    {
      CosNaming::NamingContext_var quoterNamingContext_var =
        this->get_naming_context (factory_key,
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ** now a proper reference to the quoter naming context is
      // available

      // Fill in the name of the Quoter Factory.
      // Take the key supplied to search for a Quoter Factory
      CosNaming::Name factory_Name = (CosNaming::Name) factory_key;

      // Try to get a reference to a Quoter Factory
      CORBA::Object_var quoterFactoryObject_var =
        quoterNamingContext_var->resolve (factory_Name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // We were able to get a reference to Quoter Factory.
      // Check if it is a valid Quoter Factory reference
      if (CORBA::is_nil (quoterFactoryObject_var.in()))
        { // throw a NoFactory exception
          ACE_TRY_THROW (CosLifeCycle::NoFactory (factory_key));
        }

      Stock::Quoter_Factory_var factory_var =
        Stock::Quoter_Factory::_narrow (quoterFactoryObject_var.in (),
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory_var.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "invalid factory.\n"));
          return CORBA::Object::_nil ();
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Factory reference OK.\n"));

      // Now retrieve the Quoter obj ref corresponding to the key.
      Stock::Quoter_var quoter_var =
        factory_var->create_quoter ("test",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Quoter Created\n"));

      if (CORBA::is_nil (quoter_var.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "null quoter objref returned by factory\n"));
          return CORBA::Object::_nil ();
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Return a object reference to a new object.\n"));

      return quoter_var._retn ();
    }
  ACE_CATCHANY
    {
      ACE_THROW_RETURN (CosLifeCycle::NoFactory (factory_key),
                        CORBA::Object::_nil ());
    }
  ACE_ENDTRY;
  return CORBA::Object::_nil ();
}
