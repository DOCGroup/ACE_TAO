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

// Main program

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager m;

      // Initialize the ORB.
      m.init_child_poa (argc,
                        argv,
                        "child_poa",
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Using naming server.
      TAO_Naming_Server my_name_server;
      if (my_name_server.init (m.orb (),
                               m.child_poa ()) < 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "Name init done\n"));

      // Create PropertySetDef object and register.
      TAO_PropertySetDef *propsetdef_impl;
      ACE_NEW_RETURN (propsetdef_impl,
                      TAO_PropertySetDef,
                      -1);
      CosPropertyService::PropertySetDef_var propsetdef =
        propsetdef_impl->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name propsetdef_name (1);
      propsetdef_name.length (1);
      propsetdef_name[0].id = CORBA::string_dup ("PropertySetDef");
      my_name_server->bind (propsetdef_name,
                            propsetdef.in (),
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Make this IOR as one of the properties in there.
      CORBA::Any any_val;
      any_val <<= propsetdef_impl->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
#if 0
      // Make this IOR as one of the properties in there.
      CORBA::Object_var ior = propsetdef_impl->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Object_ptr ior_ptr = ior.in ();
      any_val.replace (CORBA::_tc_Object,
                       &ior_ptr,
                       CORBA::B_TRUE,
                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
#endif
      
      propsetdef_impl->define_property_with_mode ("PropertySetDef_IOR",
                                                  any_val,
                                                  CosPropertyService::fixed_readonly,
                                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Create PropertySet factory and then register.
      TAO_PropertySetFactory *propset_factory_impl;
      ACE_NEW_RETURN (propset_factory_impl,
                      TAO_PropertySetFactory,
                      -1);
      CosPropertyService::PropertySetFactory_var propset_factory =
        propset_factory_impl->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name propset_factory_name (1);
      propset_factory_name.length (1);
      propset_factory_name[0].id = CORBA::string_dup ("PropertySetFactory");
      my_name_server->bind (propset_factory_name,
                            propset_factory.in (),
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB Event loop.
      m.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return 1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return 1;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "\nServer is terminating"));
  return 0;
}
