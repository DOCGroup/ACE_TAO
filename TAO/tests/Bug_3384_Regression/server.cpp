#include <cstdio>
#include <memory>

#include "Server_i.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      {
        CORBA::Any scope_as_any;
        scope_as_any <<= Messaging::SYNC_NONE;

        TAO::Utils::PolicyList_Destroyer policies (1);
        policies.length (1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              scope_as_any);

        policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
      }

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      Server *server_impl {};
      ACE_NEW_RETURN (server_impl,
                      Server (orb.in ()),
                      1);
      // ServantBase_var takes ownership of the raw servant pointer.
      PortableServer::ServantBase_var owner_transfer (server_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var const server_object = root_poa->id_to_reference (id.in ());

      Test::Server_var const test_server = Test::Server::_narrow (server_object.in ());

      CORBA::String_var const ior = orb->object_to_string (test_server.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
