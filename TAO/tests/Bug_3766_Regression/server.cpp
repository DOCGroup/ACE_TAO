// $Id$

#include "TestI.h"
#include "ace/OS.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      Test_i *test_impl = new Test_i;

      PortableServer::ServantBase_var owner_transfer(test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test_var test = Test::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (test.in());

      // Output the IOR to the <ior_output_file>
      FILE* file = ACE_OS::fopen("server.ior", "w");

      if (file == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) could not open server.ior\n"),
                          1);
      }

      ACE_OS::fprintf(file, "%s", ior.in ());
      ACE_OS::fclose(file);


      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
