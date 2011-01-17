// $Id$

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("usage: %s -k <ior>\n"),
                           argv [0]), -1);
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

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA", poa_manager.in (), policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior);

      Simple_Server_var server = Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%C> is nil\n",
                             ior),
                            1);
        }

      Callback_i callback_impl (orb.in ());

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("client_callback");

      child_poa->activate_object_with_id (id.in (), &callback_impl);

      CORBA::Object_var callback_object =
        child_poa->id_to_reference (id.in ());

      Callback_var callback = Callback::_narrow (callback_object.in ());

      CORBA::String_var ior = orb->object_to_string (callback.in ());

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client callback activated as <%C>\n",
                            ior.in ()));

      // Send the calback object to the server
      server->callback_object (callback.in ());

      int pre_call_connections =
        orb->orb_core ()->lane_resources ().transport_cache ().current_size ();

      // A method to kickstart callbacks from the server
      CORBA::Long r = server->test_method (1);

      if (r != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) unexpected result = %d ",
                      r));
        }

      orb->run ();

      int cur_connections =
        orb->orb_core ()->lane_resources ().transport_cache ().current_size ();

      if (cur_connections > pre_call_connections)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Expected %d "
                      "connections in the transport cache, but found "
                      "%d instead.  Aborting.\n",
                      pre_call_connections,
                      cur_connections));
          ACE_OS::abort ();
        }

      root_poa->destroy (1, 1);
    }
  catch (CORBA::Exception &excep)
    {
      excep._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
