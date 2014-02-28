// $Id$

#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/GOA.h"



const ACE_TCHAR *uipmc_url = 0;
const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:u:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'u':
        uipmc_url = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-u <miop_url>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Wrong arguments\n"), -1);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      Server_impl* server_obj = 0;
      ACE_NEW_RETURN (server_obj,
                      Server_impl(orb.in()),
                      -1);
      PortableServer::ServantBase_var owner (server_obj);

      CORBA::String_var ior;
      if (uipmc_url)
        {
          PortableGroup::GOA_var root_poa =
            PortableGroup::GOA::_narrow (poa_object.in ());

          if (CORBA::is_nil (root_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Panic: nil RootPOA\n"),
                              1);

          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager ();

          // create UIPMC reference
          CORBA::Object_var ref =
            orb->string_to_object (uipmc_url);

          // create id
          PortableServer::ObjectId_var id =
            root_poa->create_id_for_reference (ref.in ());

          // activate object
          root_poa->activate_object_with_id (id.in (),
                                             server_obj);

          ior =
            orb->object_to_string (ref.in ());

          poa_manager->activate ();
        }
      else
        {
          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (poa_object.in ());

          if (CORBA::is_nil (root_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Panic: nil RootPOA\n"),
                              1);

          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager ();

          CORBA::Object_var ref =
            server_obj->_this ();

          ior =
            orb->object_to_string (ref.in ());

          poa_manager->activate ();
        }

      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();

      orb->destroy ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception in server.cpp:\n");
      return -1;
    }

  return 0;
}
