// $Id$

#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "../check_supported_priorities.cpp"
#include "tao/Strategies/advanced_resource.h"

class Worker_Thread : public ACE_Task_Base
{
  // = TITLE
  //   Runs one client worker thread.
  //
  // = DESCRIPTION
  //   Sets ClientProtocolPolicy override to the specified value on
  //   the Current level, sets thread priority to the specified value
  //   and waits on barrier for other worker threads.  Then loops
  //   making invocations on the specified server object.
  //
public:
  Worker_Thread (CORBA::ORB_ptr orb,
                 Test_ptr server,
                 CORBA::ULong protocol_type,
                 ACE_Barrier *barrier);
  // Constructor.

  virtual int svc (void);
  // Do work.

private:
  CORBA::ORB_ptr orb_;
  // ORB.

  Test_ptr server_;
  // The server.

  CORBA::ULong protocol_type_;
  // Protocol for ClientProtocolPolicy.

  ACE_Barrier *synchronizer_;
  // Mechanism for synchronization with other worker threads.
};

// ****************************************************************
const char *ior = "file://test.ior";
int iterations = 100;
CORBA::Short priority1 = -1;
CORBA::Short priority2 = -1;
CORBA::ULong protocol1 = 1413566210;
CORBA::ULong protocol2 = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:a:b:e:f:n:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case 'a':
        result = ::sscanf (get_opts.opt_arg (),
                           "%hd",
                           &priority1);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;
      case 'b':
        result = ::sscanf (get_opts.opt_arg (),
                           "%hd",
                           &priority2);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        break;
      case 'e':
        result = ::sscanf (get_opts.opt_arg (),
                           "%u",
                           &protocol1);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-e> option"),
                            -1);
        break;
      case 'f':
        result = ::sscanf (get_opts.opt_arg (),
                           "%u",
                           &protocol2);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-f> option"),
                            -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior> "
                           "-a <priority1> "
                           "-b <priority2> "
                           "-e <protocol_type1> "
                           "-f <protocol_type2> "
                           "-n <number_of_iterations> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (priority1 < 0
      || priority2 < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Valid thread priorities must be specified.\n"
                           "See README for command-line options.\n"),
                          -1);
  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
main (int argc, char *argv[])
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // Priority Mapping Manager.
      CORBA::Object_var object =
        orb->resolve_initial_references ("PriorityMappingManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (mapping_manager.in (), "Mapping Manager") == -1)
        return 1;

      RTCORBA::PriorityMapping *pm =
        mapping_manager->mapping ();

      // RTCurrent.
      object =
        orb->resolve_initial_references ("RTCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (current.in (), "RTCurrent") == -1)
        return 1;

      // Obtain Test object reference.
      object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test_var server = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (server.in (), "Test object") == -1)
        return 1;

      // Check that test object is configured with CLIENT_PROPAGATED
      // PriorityModelPolicy.
      CORBA::Policy_var policy =
        server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::PriorityModelPolicy_var priority_policy =
        RTCORBA::PriorityModelPolicy::_narrow (policy.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
        return 1;

      RTCORBA::PriorityModel priority_model =
        priority_policy->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (priority_model != RTCORBA::CLIENT_PROPAGATED)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: priority_model != "
                           "RTCORBA::CLIENT_PROPAGATED!\n"),
                          1);

      // Spawn two worker threads.
      ACE_Barrier barrier (2);
      int flags  = THR_NEW_LWP|THR_JOINABLE|THR_BOUND;

      // Worker 1.
      Worker_Thread worker1 (orb.in (),
                             server.in (),
                             protocol1,
                             &barrier);

      CORBA::Short native_priority1 = 0;
      if (pm->to_native (priority1, native_priority1) == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
             "Cannot convert corba priority %d to native priority\n",
                               priority1),
                              1);

      if (worker1.activate (flags,
                            1, 1,
                            native_priority1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate first client worker threads\n"),
                          1);

      // Worker 2.
      Worker_Thread worker2 (orb.in (),
                             server.in (),
                             protocol2,
                             &barrier);

      CORBA::Short native_priority2 = 0;
      if (pm->to_native (priority2, native_priority2) == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
             "Cannot convert corba priority %d to native priority\n",
                               priority2),
                              1);

      if (worker2.activate (flags,
                            1, 1,
                            native_priority2) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate second client worker threads\n"),
                          1);

      // Wait for worker threads to finish.
      ACE_Thread_Manager::instance ()->wait ();

      // Testing over.  Shut down the server.
      ACE_DEBUG ((LM_DEBUG, "Client threads finished\n"));
      current->the_priority (priority1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
       "Unexpected exception in MT_Client_Protocol_Priority test client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
// ****************************************************************

Worker_Thread::Worker_Thread (CORBA::ORB_ptr orb,
                              Test_ptr server,
                              CORBA::ULong protocol_type,
                              ACE_Barrier *barrier)
  : orb_ (orb),
    server_ (server),
    protocol_type_ (protocol_type),
    synchronizer_ (barrier)
{
}

int
Worker_Thread::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 0;

      // PolicyCurrent.
      object =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (policy_current.in (), "PolicyCurrent")
          == -1)
        return 0;

      // Set ClientProtocolPolicy override on the Current.
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = this->protocol_type_;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for other threads.
      this->synchronizer_->wait ();

      for (int i = 0; i < iterations; ++i)
        {
          // Invoke method.
          this->server_->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Worker Thread exception:");
    }
  ACE_ENDTRY;
  return 0;
}
