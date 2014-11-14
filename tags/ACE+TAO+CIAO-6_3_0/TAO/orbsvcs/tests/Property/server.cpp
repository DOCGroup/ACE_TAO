
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  Test server for the CosPropertyService
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#include "server.h"



// Main program

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      // Using naming server.
      TAO_Naming_Client my_name_client;
      if (my_name_client.init (orb.in ()) < 0)
        return 1;

      // Create PropertySetDef object and register.
      TAO_PropertySetDef *propsetdef_impl = 0;
      ACE_NEW_RETURN (propsetdef_impl,
                      TAO_PropertySetDef,
                      -1);
      CosPropertyService::PropertySetDef_var propsetdef =
        propsetdef_impl->_this ();

      CosNaming::Name propsetdef_name (1);
      propsetdef_name.length (1);
      propsetdef_name[0].id = CORBA::string_dup ("PropertySetDef");
      my_name_client->bind (propsetdef_name,
                            propsetdef.in ());
      CORBA::Any any_val;

      // Make this IOR as one of the properties in there.
      any_val <<= propsetdef.in ();

      CORBA::Object_var ior = CosPropertyService::PropertySetDef::_duplicate (propsetdef.in());
      CORBA::Object_ptr ior_ptr = ior.in ();
      any_val <<= ior_ptr;

      propsetdef_impl->define_property_with_mode ("PropertySetDef_IOR",
                                                  any_val,
                                                  CosPropertyService::fixed_readonly);

      // Create PropertySet factory and then register.
      TAO_PropertySetFactory *propset_factory_impl;
      ACE_NEW_RETURN (propset_factory_impl,
                      TAO_PropertySetFactory,
                      -1);
      CosPropertyService::PropertySetFactory_var propset_factory =
        propset_factory_impl->_this ();

      CosNaming::Name propset_factory_name (1);
      propset_factory_name.length (1);
      propset_factory_name[0].id = CORBA::string_dup ("PropertySetFactory");
      my_name_client->bind (propset_factory_name,
                            propset_factory.in ());

      poa_manager->activate ();

      // Run the ORB Event loop.
      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return 1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG,
              "\nServer is terminating"));
  return 0;
}
