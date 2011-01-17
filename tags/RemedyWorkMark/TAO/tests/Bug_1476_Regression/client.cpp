// $Id$

#include "Client_Task.h"
#include "Server_Task.h"
#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/AnyTypeCode/Any.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int number_of_oneways = 10;
int number_of_client_tasks = 2;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n' :
        number_of_oneways = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'c' :
        number_of_client_tasks = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "-n <number of oneways>"
                           "-c <number of client tasks>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_NONE;

      CORBA::PolicyList policies (1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the sender reference..
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Sender_var sender =
        Test::Sender::_narrow(tmp.in ());

      if (CORBA::is_nil (sender.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             ior),
                            1);
        }

      Client_Task client_task (orb.in(),
                               sender.in (),
                               ACE_Thread_Manager::instance (),
                               number_of_oneways);

      Server_Task server_task (orb.in (),
                               ACE_Thread_Manager::instance ());

      // Before creating threads we will let the sender know that we
      // will have two threads that would make invocations..
      // this is the first oneway we do, so after this we would have a queue
      // on one of the transports
      sender->active_objects ((CORBA::Short) number_of_client_tasks);

      TAO::Transport_Cache_Manager& manager = orb->orb_core()->lane_resources ().transport_cache ();

      TAO::Transport_Cache_Manager::HASH_MAP& cachemap = manager.map();
      TAO::Transport_Cache_Manager::HASH_MAP_ITER end_iter = cachemap.end ();

      bool tranportwithqueue = false;
      for (TAO::Transport_Cache_Manager::HASH_MAP_ITER iter = cachemap.begin ();
           iter != end_iter;
           ++iter)
        {
          TAO_Transport* transport = (*iter).int_id_.transport ();
          if (transport->queue_is_empty() == 0 && transport->is_connected() == false)
            {
              tranportwithqueue = true;
              break;
            }
        }

        if (!tranportwithqueue)
          {
            ACE_ERROR ((LM_ERROR, "(%P|%t) Error: expect a not connected transport with a queue\n"));
          }
        else
          {
            ACE_DEBUG((LM_DEBUG, "(%P|%t) Found not connected transport with a queue\n"));
          }

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 2,1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, number_of_client_tasks, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "Event Loop finished\n"));

      orb->destroy ();

      if (tranportwithqueue == false)
      {
        return 1;
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
