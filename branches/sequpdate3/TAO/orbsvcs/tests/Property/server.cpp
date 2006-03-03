// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/CosPropertyService
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    Test server for the CosPropertyService
//
// = AUTHORS
//   Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include "server.h"

ACE_RCSID(CosPropertyService, server, "$Id$")

// Main program

int
main (int argc, char ** argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      TAO_ORB_Manager m;

      // Initialize the ORB.
      m.init_child_poa (argc,
                        argv,
                        "child_poa"
                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Using naming server.
      TAO_Naming_Client my_name_client;
      if (my_name_client.init (m.orb ()) < 0)
        return 1;

      // Create PropertySetDef object and register.
      TAO_PropertySetDef *propsetdef_impl;
      ACE_NEW_RETURN (propsetdef_impl,
                      TAO_PropertySetDef,
                      -1);
      CosPropertyService::PropertySetDef_var propsetdef =
        propsetdef_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name propsetdef_name (1);
      propsetdef_name.length (1);
      propsetdef_name[0].id = CORBA::string_dup ("PropertySetDef");
      my_name_client->bind (propsetdef_name,
                            propsetdef.in ()
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Any any_val;

      // Make this IOR as one of the properties in there.
      any_val <<= propsetdef_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var ior = propsetdef_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_ptr ior_ptr = ior.in ();
      any_val <<= ior_ptr;
      // any_val.replace (CORBA::_tc_Object,
      //                  &ior_ptr,
      //                  1
      //                  ACE_ENV_ARG_PARAMETER);
      // ACE_TRY_CHECK;

      propsetdef_impl->define_property_with_mode ("PropertySetDef_IOR",
                                                  any_val,
                                                  CosPropertyService::fixed_readonly
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create PropertySet factory and then register.
      TAO_PropertySetFactory *propset_factory_impl;
      ACE_NEW_RETURN (propset_factory_impl,
                      TAO_PropertySetFactory,
                      -1);
      CosPropertyService::PropertySetFactory_var propset_factory =
        propset_factory_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name propset_factory_name (1);
      propset_factory_name.length (1);
      propset_factory_name[0].id = CORBA::string_dup ("PropertySetFactory");
      my_name_client->bind (propset_factory_name,
                            propset_factory.in ()
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run the ORB Event loop.
      m.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,
                           "System Exception");
      return 1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                           "User Exception");
      return 1;
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "\nServer is terminating"));
  return 0;
}
