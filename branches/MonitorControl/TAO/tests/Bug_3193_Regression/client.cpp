// $Id$

#include "testC.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(Timeout, client, "$Id$")

const char *ior = "file://test.ior";
int min_timeout = 20;
int inv_count = 100;
int timeouts_received = 0;
int sleep_time = 100; // msec

class OPool : public ACE_Task_Base
{
private:
  CORBA::ORB_var orb_;

public:
  OPool (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
    if (CORBA::is_nil (orb_.in()) == 1) {
      ACE_ERROR ((LM_ERROR, "OPool> Ctr> Orb is NULL\n"));
    }
  };

  virtual ~OPool ()
  { };

  virtual int svc ()
  {
    if (CORBA::is_nil (orb_.in()) == 0) {
      orb_->run();
    }

    return -1;
  };
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:l:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'l':
        min_timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        inv_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-l <timeout> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

char* aStr;

void
send_echo (Simple_Server_ptr server,
           CORBA::Long msec)
{
  try
    {
      server->echo (0, msec, aStr);
    }
  catch (const CORBA::TIMEOUT& )
    {
      // Trap this exception and continue...
      ACE_DEBUG ((LM_DEBUG,
                  "==> Trapped a TIMEOUT exception \n"));

      timeouts_received++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
}


int main (int argc, char* argv[])
{
  aStr = new char [5000];
  memset(aStr,'A',4999);
  aStr[4999] = 0;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      int orb_thr_pool = 1;
      OPool orbPool (orb.in());
      if (orb_thr_pool > 0) {
        orbPool.activate (THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED, orb_thr_pool);
      }

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior);

      Simple_Server_var server = Simple_Server::_narrow (object.in ());
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // convert from msec to "TimeT" (0.1 usec units)
      TimeBase::TimeT mid_value = 10000 * min_timeout;

      CORBA::Any any_object;
      any_object <<= mid_value; // - 20000; // midvalue + 2 msec

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                            any_object);
      object =
        server->_set_policy_overrides (policy_list,
                                       CORBA::SET_OVERRIDE);

      Simple_Server_var timeout_server = Simple_Server::_narrow (object.in ());

      policy_list[0]->destroy ();
      policy_list.length(0);

      //for(int i = 0; i != 1000000;++i)
      for(int i = 0; i != inv_count;++i)
        {
          if( (i % 10) == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "client(%P) sent %d messages\n",i));
            }
          send_echo (timeout_server.in (), sleep_time);
        }

        ACE_DEBUG ((LM_DEBUG, "Invocations: %d, Timeouts received: %d\n"
                    , inv_count, timeouts_received));

        // Set the policy value.
        Messaging::SyncScope sync_with_target = Messaging::SYNC_WITH_TARGET;
        CORBA::Any sync_with_target_any;
        sync_with_target_any <<= sync_with_target;

        policy_list.length (1);
        policy_list[0] =
          orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_with_target_any);
        // Apply the policy at the ORB level.
        CORBA::Object_var obj = orb->resolve_initial_references ("ORBPolicyManager");
        CORBA::PolicyManager_var policy_manager =
          CORBA::PolicyManager::_narrow(obj.in());

        policy_manager->set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE);
        // Destroy the Policy objects.
        for (CORBA::ULong i = 0; i < policy_list.length(); ++i) {
          policy_list[i]->destroy ();
        }
        policy_list.length(0);

      ACE_DEBUG ((LM_DEBUG,
                  "\n\n\nclient(%P) Test completed, "
                  "resynch with server\n"));

      send_echo (timeout_server.in (), 0);

      server->shutdown ();

      orb->destroy ();

      orbPool.wait();

    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: caught CORBA::Exception\n"));
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  if (timeouts_received < 1) {
    // We should have received one timeout for the single SYNCH_WITH_TARGET case.
    ACE_ERROR ((LM_ERROR, "ERROR: No Timeout received for SYNCH_WITH_TARGET scope.\n"));
    return -1;
  }
  else if (timeouts_received < 2) {
    // We didn't receive any timeouts for the SYNC_WITH_TRANSPORT case.
    ACE_ERROR ((LM_ERROR, "ERROR: No Timeout received for SYNC_WITH_TRANSPORT scope.\n"));
    return -1;
  }

  return 0;
}
