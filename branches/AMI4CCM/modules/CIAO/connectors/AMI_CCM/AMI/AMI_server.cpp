// $Id$

#include "AMI_server.h"

AMI_server::AMI_server (::CCM_AMI::MyFoo_ptr foo_receiver)
  : foo_receiver_ (::CCM_AMI::MyFoo::_duplicate (foo_receiver))
{
}

int AMI_server::svc ()
{
  try
    {
      int argc = 2;
      ACE_TCHAR **argv = new ACE_TCHAR *[argc];
      argv[0] = ACE::strnew (ACE_TEXT (""));
      argv[1] = ACE::strnew (ACE_TEXT (""));
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ACE_TEXT ("AMI_server"));

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          printf ("Server :\tERROR creating POA \n");
          return 1;
        }
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      AMI_internal_i ami_internal_i (orb.in (), foo_receiver_);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&ami_internal_i);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      CCM_AMI::MyFoo_var ami_foo_var =
        CCM_AMI::MyFoo::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (ami_foo_var.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen ("server.ior", "w");
      if (output_file == 0)
        {
          printf ("Cannot open output file for writing IOR: server.ior\n");
          return 1;
        }
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      printf ("AMI CORBA :\tServer is activated\n");

      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
