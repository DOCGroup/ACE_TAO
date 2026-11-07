#include "Client_i.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "tao/Utils/PolicyList_Destroyer.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
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
  return 0;
}

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);
  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;
  // ctor

  int svc () override;
  // The thread entry point.

private:
  CORBA::ORB_var const orb_;
  // The orb
};

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int result {1};
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");

  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const server_object = orb->string_to_object (ior);

      Test::Server_var const test_server = Test::Server::_narrow (server_object.in ());

      if (CORBA::is_nil (test_server.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "ERROR: Nil reference in Test::Server reference <%s>\n",
                           ior),
                          1);

      CORBA::Object_var object_with_policy;
      {
        CORBA::Any scope_as_any;
        scope_as_any <<= Messaging::SYNC_NONE;

        TAO::Utils::PolicyList_Destroyer policies (1);
        policies.length (1);
        policies[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              scope_as_any);

        object_with_policy =
          test_server->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
      }

      Test::Server_var const test_server_no_sync =
        Test::Server::_narrow (object_with_policy.in ());

      if (CORBA::is_nil (test_server_no_sync.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "ERROR: Nil reference in Test::Server reference <%s>\n",
                           ior),
                          1);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client server thread\n"),
                          1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test starting . . .\n"));

      // The RootPOA borrows this servant until it is destroyed.
      Client client_impl;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&client_impl);

      CORBA::Object_var const client_object = root_poa->id_to_reference (id.in ());

      Test::Client_var const test_client = Test::Client::_narrow (client_object.in ());

      // setup client callback at server
      test_server_no_sync->setup (test_client.in ());

      // send oneway request to server
      test_server_no_sync->request (1);

      // sleep 2 sec to give ample opportunity for oneway reply to be received by worker
      ACE_OS::sleep (2);

      // check if reply received
      if (client_impl.reply_count () > 0)
      {
        ACE_DEBUG ((LM_INFO, "(%P|%t) Oneway reply correctly received\n"));

        result = 0; // test OK
      }
      else
      {
        ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Oneway reply not received\n"));

        // send second request to trigger reception of first and second reply
        test_server_no_sync->request (2);

        // sleep 2 sec to give ample opportunity for oneway reply to be received by worker
        ACE_OS::sleep (2);

        if (client_impl.reply_count () > 1)
        {
          ACE_DEBUG ((LM_INFO, "(%P|%t) Received both replies after second request\n"));
        }
        else
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) FATAL ERROR: Still no replies received\n"));
        }
      }

      // shutdown server (use original synchronous reference to be sure to deliver message)
      test_server->shutdown ();

      // shutdown worker
      orb->shutdown (true);

      worker.thr_mgr ()->wait ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
    }
  return 0;
}
