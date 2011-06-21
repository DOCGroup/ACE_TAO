
//=============================================================================
/**
 *  @file    Generic_Factory_i.cpp
 *
 *  $Id$
 *
 *  The implementation of a LifeCycle Service GenericFactory for the
 *  Quoter example.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "Generic_Factory_i.h"
#include "QuoterC.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"

// Constructor
Quoter_Generic_Factory_i::Quoter_Generic_Factory_i (int debug_level)
: debug_level_ (debug_level)
{
}

// Destructor.
Quoter_Generic_Factory_i::~Quoter_Generic_Factory_i (void)
{
}

CORBA::Boolean
Quoter_Generic_Factory_i::supports (const CosLifeCycle::Key &)
{
  return 0;
}



CosNaming::NamingContext_ptr
Quoter_Generic_Factory_i::get_naming_context (const CosLifeCycle::Key &factory_key)
{
  CosNaming::NamingContext_var quoterNamingContext_var;
  try
    {
      // @@ FIXME Get a reference to the ORB.
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
        CosNaming::NamingContext::_narrow (namingObj_var.in ());

      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      // Get the IDL_Quoter naming context.
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName);

      quoterNamingContext_var =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in ());
    }
  catch (const CORBA::Exception&)
    {
      throw CosLifeCycle::NoFactory (factory_key);
    }
  return quoterNamingContext_var._retn ();
}



CORBA::Object_ptr
Quoter_Generic_Factory_i::create_object (const CosLifeCycle::Key &factory_key,
                                         const CosLifeCycle::Criteria &)
{
  Stock::Quoter_var quoter_var;
  try
    {
      CosNaming::NamingContext_var quoterNamingContext_var =
        this->get_naming_context (factory_key);

      // ** now a proper reference to the quoter naming context is
      // available

      // Fill in the name of the Quoter Factory.
      // Take the key supplied to search for a Quoter Factory
      CosNaming::Name factory_Name = (CosNaming::Name) factory_key;

      // Try to get a reference to a Quoter Factory
      CORBA::Object_var quoterFactoryObject_var =
        quoterNamingContext_var->resolve (factory_Name);

      // We were able to get a reference to Quoter Factory.
      // Check if it is a valid Quoter Factory reference
      if (CORBA::is_nil (quoterFactoryObject_var.in()))
        { // throw a NoFactory exception
          throw CosLifeCycle::NoFactory (factory_key);
        }

      Stock::Quoter_Factory_var factory_var =
        Stock::Quoter_Factory::_narrow (quoterFactoryObject_var.in ());

      if (CORBA::is_nil (factory_var.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "invalid factory.\n"));
          return CORBA::Object::_nil ();
        }

      if (this->debug_level_ > 1)
        ACE_DEBUG ((LM_DEBUG, "Generic Factory: Factory reference OK.\n"));

      // Now retrieve the Quoter obj ref corresponding to the key.
      quoter_var =
        factory_var->create_quoter ("test");

      if (this->debug_level_ > 1)
        ACE_DEBUG ((LM_DEBUG, "Generic_Factory: Quoter Created\n"));

      if (CORBA::is_nil (quoter_var.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "null quoter objref returned by factory\n"));
          return CORBA::Object::_nil ();
        }

      if (this->debug_level_ > 1)
        ACE_DEBUG ((LM_DEBUG, "Generic_Factory: Return an object reference to a new object.\n"));

    }
  catch (const CORBA::Exception&)
    {
      throw CosLifeCycle::NoFactory (factory_key);
    }
  return quoter_var._retn ();
}
