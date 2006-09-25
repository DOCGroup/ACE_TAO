// $Id$

#include "testC.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Timeout, client, "$Id$")

const char *ior = "file://test.ior";
int min_timeout = 0;
int max_timeout = 20;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:l:h:");
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

      case 'h':
        max_timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-l <min_timeout> "
                           "-h <max_timeout> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

enum TO_TYPE {none = 0, orb1 = 1, thread1 = 2, object1 = 3};
char *to_type_names[4] ={"none", "orb", "thread", "object"};

static int timeout_count[4] = {0, 0, 0, 0};
static int in_time_count[4] = {0, 0, 0, 0};

void
send_echo (TO_TYPE ctype, CORBA::ORB_ptr orb,
           Simple_Server_ptr server,
           CORBA::Long t
           ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      server->echo (0, t ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      in_time_count[ctype]++;
    }
  ACE_CATCH (CORBA::TIMEOUT, timeout)
    {
      timeout_count[ctype]++;

      // Trap this exception and continue...
      ACE_DEBUG ((LM_DEBUG,
                  "==> Trapped a TIMEOUT exception (expected)\n"));

      // Sleep so the server can send the reply...
      ACE_Time_Value tv (max_timeout / 1000, // max_timeout is in msec, so get seconds
                         (max_timeout % 1000) * 1000); // and usec

      // This is a non-standard TAO call that's used to give the
      // client ORB a chance to cleanup the reply that's come back
      // from the server.
      orb->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
}


int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("PolicyCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TimeBase::TimeT mid_value =
        10000 * (min_timeout + max_timeout) / 2; // convert from msec to "TimeT" (0.1 usec units)

      CORBA::Any any_orb;
      any_orb <<= mid_value;
      CORBA::Any any_thread;
      any_thread <<= mid_value + 10000; // midvalue + 1 msec
      CORBA::Any any_object;
      any_object <<= mid_value + 20000; // midvalue + 2 msec

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                            any_object
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      object =
        server->_set_policy_overrides (policy_list,
                                       CORBA::SET_OVERRIDE
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var timeout_server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_list[0] = CORBA::Policy::_nil ();

      ACE_DEBUG ((LM_DEBUG,
                  "client (%P) testing from %d to %d milliseconds\n",
                  min_timeout, max_timeout));

      for (CORBA::Long t = min_timeout; t < max_timeout; ++t)
       {
         ACE_DEBUG ((LM_DEBUG,
                     "\n================================\n"
                     "Trying with timeout = %d msec\n", t));

         ACE_DEBUG ((LM_DEBUG,
                     "Cleanup ORB/Thread/Object policies\n"));

          policy_list.length (0);
          policy_manager->set_policy_overrides (policy_list,
                                                CORBA::SET_OVERRIDE
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          policy_current->set_policy_overrides (policy_list,
                                                CORBA::SET_OVERRIDE
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          send_echo (none, orb.in (), server.in (), t ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;




          ACE_DEBUG ((LM_DEBUG,
                      "client(%P) Set the ORB policies\n"));

          policy_list.length (1);
          policy_list[0] =
            orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                any_orb
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          policy_manager->set_policy_overrides (policy_list,
                                                CORBA::SET_OVERRIDE
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          send_echo (orb1, orb.in (), server.in (), t ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;







          ACE_DEBUG ((LM_DEBUG,
                      "client(%P) Set the thread policies\n"));

          policy_list.length (1);
          policy_list[0] =
            orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                any_thread
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          policy_current->set_policy_overrides (policy_list,
                                                CORBA::SET_OVERRIDE
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          send_echo (thread1, orb.in (), server.in (), t ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;







          ACE_DEBUG ((LM_DEBUG,
                      "client(%P) Use the object policies\n"));
          send_echo (object1, orb.in (), timeout_server.in (), t ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "\n\n\nclient(%P) Test completed, "
                  "resynch with server\n"));
      policy_list.length (0);
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      send_echo (none, orb.in (), server.in (), 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int timeout_count_total = 0;
      int in_time_count_total = 0;
      for (int i = 0; i < 4; i++) {
        timeout_count_total += timeout_count[i];
        in_time_count_total += in_time_count[i];
        ACE_DEBUG ((LM_DEBUG, "in_time_count[%s]= %d timeout_count[%s]= %d\n",
                    to_type_names[i], in_time_count[i],
                    to_type_names[i], timeout_count[i]));
      }

      if (timeout_count_total == 0)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: No messages timed out\n"));

      if (in_time_count_total == 0)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: No messages on time (within time limit)\n"));

      ACE_DEBUG ((LM_DEBUG, "in_time_count_total = %d, timeout_count_total = %d\n",
                  in_time_count_total, timeout_count_total));

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
