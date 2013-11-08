// $Id$

#include <stdio.h>
#include <iostream>
#include <omg/PortableServer.hh>
#include "test_i.h"
#include "Server_ORBInitializer.h"

const char* ior_output_file = "test.ior";

//FUZZ: disable check_for_improper_main_declaration
int
main(int argc, char* argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_var initializer = new Server_ORBInitializer();

      PortableInterceptor::register_orb_initializer (initializer.in ());

      // Now we can create the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "taodemo");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      assert(!CORBA::is_nil (poa_object.in ()));

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CDR_Out_Arg_i server_impl(orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var test_obj =
        root_poa->id_to_reference (id.in ());

      Interop::CDR_Out_Arg_var server =
        Interop::CDR_Out_Arg::_narrow (test_obj.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      printf("Interop::CDR_Out_Arg: <%s>\n",
                  ior.in ());

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= fopen (ior_output_file, "w");
          if (output_file == 0) {
            printf("Cannot open output file for writing IOR: %s\n",
                               ior_output_file);
            return 1;
          }
          fprintf (output_file, "%s", ior.in ());
          fclose (output_file);
        }

      orb->run ();

      printf("event loop finished\n");

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      std::cerr << "Caught exception in server:" << ex << std::endl;
      return 1;
    }

  return 0;
}
