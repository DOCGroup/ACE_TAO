#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *uipmc_url {};
  ACE_TCHAR const *ior_output_file {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:u:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'u':
        options.uipmc_url = get_opts.opt_arg ();
        break;
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Wrong arguments\n"), -1);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      // ACE_NEW_RETURN needs a writable raw pointer; owner adopts the servant reference.
      Server_impl *server_obj {};
      ACE_NEW_RETURN (server_obj,
                      Server_impl (orb.in ()),
                      -1);
      PortableServer::ServantBase_var const owner (server_obj);

      CORBA::String_var ior;
      if (options.uipmc_url != nullptr)
        {
          PortableGroup::GOA_var const root_poa =
            PortableGroup::GOA::_narrow (poa_object.in ());

          if (CORBA::is_nil (root_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Panic: nil RootPOA\n"),
                              1);

          PortableServer::POAManager_var const poa_manager =
            root_poa->the_POAManager ();

          CORBA::Object_var const ref =
            orb->string_to_object (options.uipmc_url);

          PortableServer::ObjectId_var const id =
            root_poa->create_id_for_reference (ref.in ());

          root_poa->activate_object_with_id (id.in (),
                                             server_obj);

          ior =
            orb->object_to_string (ref.in ());

          poa_manager->activate ();
        }
      else
        {
          PortableServer::POA_var const root_poa =
            PortableServer::POA::_narrow (poa_object.in ());

          if (CORBA::is_nil (root_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Panic: nil RootPOA\n"),
                              1);

          PortableServer::POAManager_var const poa_manager =
            root_poa->the_POAManager ();

          CORBA::Object_var const ref =
            server_obj->_this ();

          ior =
            orb->object_to_string (ref.in ());

          poa_manager->activate ();
        }

      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s\n",
                               ACE_TEXT_ALWAYS_CHAR (options.ior_output_file)),
                              -1);
          if (ACE_OS::fprintf (output_file.get (), "%s", ior.in ()) < 0)
            ACE_ERROR_RETURN ((LM_ERROR, "Cannot write IOR to output file\n"), -1);
        }

      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception in server.cpp:\n");
      return -1;
    }

  return 0;
}
