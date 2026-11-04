#include "TestCallback.h"
#include "ace/Get_Opt.h"

#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      // Create client callback CORBA object

      // Transfer the servant pointer to its reference-counted owner below.
      TestCallback *test_impl {};
      ACE_NEW_RETURN (test_impl,
                      TestCallback (orb.in ()),
                      1);
      PortableServer::ServantBase_var const owner_transfer (test_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::TestCallback_var const client = Test::TestCallback::_narrow (object.in ());

      poa_manager->activate ();

      // Get server CORBA object and call it

      CORBA::Object_var const tmp = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      Test::TestServer_var const server = Test::TestServer::_narrow (tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::TestServer reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - calling server\n"));
      server->pass_callback (client.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - call failed to block\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - not stopped by server\n"));
  return 1;
}
