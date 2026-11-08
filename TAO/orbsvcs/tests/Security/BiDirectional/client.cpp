#include "ace/Get_Opt.h"
#include "ace/OS_Memory.h"
#include "test_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#include <cstdlib>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
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
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any policy_value;
      policy_value <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            policy_value);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies);

      for (CORBA::ULong i {}; i < policies.length (); ++i)
        policies[i]->destroy ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior);
      Simple_Server_var const server = Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil\n",
                           options.ior),
                          1);

      Callback_i *callback_impl {};
      ACE_NEW_RETURN (callback_impl, Callback_i (orb.in ()), 1);
      // ServantBase_var owns the initial reference; the POA retains its activated reference.
      PortableServer::ServantBase_var const callback_owner (callback_impl);
      PortableServer::ObjectId_var const id =
        PortableServer::string_to_ObjectId ("client_callback");
      child_poa->activate_object_with_id (id.in (), callback_impl);

      CORBA::Object_var const callback_object =
        child_poa->id_to_reference (id.in ());
      Callback_var const callback = Callback::_narrow (callback_object.in ());
      CORBA::String_var const callback_ior = orb->object_to_string (callback.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client callback activated as <%C>\n"),
                  callback_ior.in ()));

      server->callback_object (callback.in ());

      size_t const pre_call_connections =
        orb->orb_core ()->lane_resources ().transport_cache ().current_size ();
      CORBA::Long const result = server->test_method (true);

      if (result != 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) unexpected result = %d "),
                    result));

      orb->run ();

      size_t const cur_connections =
        orb->orb_core ()->lane_resources ().transport_cache ().current_size ();

      if (cur_connections > pre_call_connections)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Expected %B connections in the transport cache, ")
                      ACE_TEXT ("but found %B instead.  Aborting.\n"),
                      pre_call_connections,
                      cur_connections));
          std::abort ();
        }

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
