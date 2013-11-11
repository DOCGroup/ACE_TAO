// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "../check_supported_priorities.cpp"

// Default IOR files.
const ACE_TCHAR *ior1 = ACE_TEXT("file://test1.ior");
const ACE_TCHAR *ior2 = ACE_TEXT("file://test2.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        ior1 = get_opts.opt_arg ();
        break;
      case 'o':
        ior2 = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <ior> "
                           "-o <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

CORBA::Short
get_server_priority (Test_ptr server)
{
  // Get the Priority Model Policy from the stub.
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

  // Narrow down to correct type.
  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

  // Make sure that we have the SERVER_DECLARED priority model.
  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model ();
  if (priority_model != RTCORBA::SERVER_DECLARED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: priority_model != "
                       "RTCORBA::SERVER_DECLARED!\n"),
                      -1);

  // Return the server priority.
  return priority_policy->server_priority ();
}

void
invocation_exception_test (Test_ptr obj,
                           CORBA::Short priority)
{
  try
    {
      // Invoke method on test object.
      obj->test_method (1,
                        priority);

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  catch (const CORBA::INV_POLICY& )
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "INV_POLICY exception is caught as expected.\n"));
    }
  catch (const CORBA::Exception&)
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "Error: unexpected exception caught\n"));
      throw;
    }
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      // Get the RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Get the RTCurrent.
      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      // We need to set the client thread CORBA priority
      current->the_priority (get_implicit_thread_CORBA_priority (this->orb_.in ()));

      // Test object 1 (with CLIENT_PROPAGATED priority model).
      object =
        this->orb_->string_to_object (ior1);

      Test_var client_propagated_obj =
        Test::_narrow (object.in ());

      // Test object 2 (with SERVER_DECLARED priority model).
      object = this->orb_->string_to_object (ior2);

      Test_var server_declared_obj =
        Test::_narrow (object.in ());

      // Test: Attempt to set priority bands that do not match server
      // resource configuration on the <client_propagated_obj>.
      // Should get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Client bands do not match server lanes\n\n"));

      // False bands.
      RTCORBA::PriorityBands false_bands;
      false_bands.length (2);
      false_bands[0].low = 10000;
      false_bands[0].high = 10005;
      false_bands[1].low = 26000;
      false_bands[1].high = 30000;
      CORBA::PolicyList policies;
      policies.length (1);
      policies[0] =
        rt_orb->create_priority_banded_connection_policy (false_bands);

      // Set false bands at the object level.  Note that a new object
      // is returned.
      object =
        client_propagated_obj->_set_policy_overrides (policies,
                                                      CORBA::SET_OVERRIDE);

      client_propagated_obj =
        Test::_narrow (object.in ());

      // Invoking on this object with false bands should produce an
      // exception.
      invocation_exception_test (client_propagated_obj.in (),
                                 0);

      // Get the correct bands from the <server_declared_obj>.
      policies[0] =
        server_declared_obj->_get_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE);

      RTCORBA::PriorityBandedConnectionPolicy_var bands_policy =
        RTCORBA::PriorityBandedConnectionPolicy::_narrow (policies[0]);

      RTCORBA::PriorityBands_var bands =
        bands_policy->priority_bands ();

      // Set the proper bands at the object level.  Note that a new
      // object is returned.
      object =
        client_propagated_obj->_set_policy_overrides (policies,
                                                      CORBA::SET_OVERRIDE);

      // Overwrite existing <client_propagated_obj>.
      client_propagated_obj =
        Test::_narrow (object.in ());

      // Test: Attempt invocation on <client_propagated_obj> with
      // client thread priority not matching any of the bands.  Should
      // get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Client threads does not match band\n\n"));

      // Choose a priority one higher than the high priority of the
      // highest band.
      CORBA::Short client_priority =
        bands[bands->length () - 1].high + 1;

      // Reset the current thread's priority.
      current->the_priority (client_priority);

      // Invoking on this object with an invalid client thread
      // priority should produce an exception.
      invocation_exception_test (client_propagated_obj.in (),
                                 client_priority);

      // Test: Make invocations on the <client_propagated_obj>.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Invoking on client propagated object\n\n"));

      // Make an invocation for each band.
      for (CORBA::ULong i = 0;
           i < bands->length ();
           ++i)
        {
          // Select a priority in the middle of the band.
          CORBA::Short client_priority =
            (bands[i].low + bands[i].high) / 2;

          // Reset the current thread's priority.
          current->the_priority (client_priority);

          // Invoke test method on server.
          client_propagated_obj->test_method (1, // CLIENT_PROPAGATED
                                              client_priority);
        }

      // Test: Attempt invocation with the same thread priority, but
      // now on the <server_declared_obj>.  This should succeed.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Invoking on server declared object\n\n"));

      // Get the <server_priority> from the stub.
      CORBA::Short server_priority =
        get_server_priority (server_declared_obj.in ());

      // Invoke test method on server.
      server_declared_obj->test_method (0, // SERVER_DECLARED
                                        server_priority);

      // Testing over. Shut down Server ORB.
      server_declared_obj->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception in Banded_Connections test client:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Parse arguments.
      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception in Banded_Connections test client:");
      return -1;
    }

  return 0;
}
