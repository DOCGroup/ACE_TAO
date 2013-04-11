// $Id$

#include "Client_i.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 1;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Server_var test_server =
        Test::Server::_narrow(tmp.in ());

      if (CORBA::is_nil (test_server.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "ERROR: Nil reference in Test::Server reference <%s>\n",
                           ior),
                          1);

      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_NONE;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);

      tmp = test_server->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      Test::Server_var test_server_no_sync =
        Test::Server::_narrow(tmp.in ());

      if (CORBA::is_nil (test_server_no_sync.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "ERROR: Nil reference in Test::Server reference <%s>\n",
                           ior),
                          1);

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

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client server thread\n"),
                          1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test starting . . .\n"));

      Client client_impl;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&client_impl);

      tmp = root_poa->id_to_reference (id.in ());

      Test::Client_var test_client =
        Test::Client::_narrow (tmp.in ());

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
      orb->shutdown (1);

      worker.thr_mgr ()->wait ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}
