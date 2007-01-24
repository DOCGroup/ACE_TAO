// $Id$

#include "Hello.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(hello_impl);

      poa_manager->activate ();

      PortableServer::ObjectId_var obj_id = root_poa->activate_object (hello_impl ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var obj_var = root_poa->id_to_reference (obj_id.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::ObjectId_var new_obj_id = root_poa->reference_to_id (obj_var.in () ACE_ENV_ARG_PARAMETER);

      // Invoke reference_to_servant(). Should retrieve servant.
      PortableServer::ServantBase_var servant =
        root_poa->reference_to_servant (obj_var.in ()
                                        ACE_ENV_ARG_PARAMETER);

      // Assert correctness.
      ACE_ASSERT (hello_impl == servant.in());

      root_poa->deactivate_object (new_obj_id.in () ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
