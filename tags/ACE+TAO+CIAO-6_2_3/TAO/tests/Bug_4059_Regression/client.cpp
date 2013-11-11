// $Id$

#include "TestC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Policy_ManagerC.h"
#include "ace/Synch.h"
#include "ace/Condition_T.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Thread.h"
#include "ace/Get_Opt.h"

TAO_SYNCH_MUTEX test_lock;
ACE_Condition<TAO_SYNCH_MUTEX> cond (test_lock);
bool is_ok = false;
CORBA::ORB_var orb;

const ACE_TCHAR *ior = ACE_TEXT ("corbaloc::localhost:5678/Test");

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

ACE_THR_FUNC_RETURN failsafe (void *)
{
  test_lock.acquire();
  ACE_Time_Value timeout = ACE_OS::gettimeofday() + ACE_Time_Value (10,0);
  cond.wait(&timeout);
  test_lock.release();
  if (!is_ok)
    {
      ACE_DEBUG ((LM_DEBUG, "FAILURE: failsafe timed out\n"));
      orb->shutdown(0);
      ACE_OS::exit(1);
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Set the tcp protocol properties
      RTCORBA::TCPProtocolProperties_var tcp_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1, 0, 1, 0);
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = 0;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_duplicate (tcp_properties.in ());
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList policy_list;

      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);
      object = orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

      // start the failsafe thread. It will expire in 10 seconds and
      // terminate with a failure if the following does not complete.

      ACE_Thread::spawn (failsafe);

      object = orb->string_to_object ("corbaloc::localhost:5678/Test");
      Test::Hello_var hello = Test::Hello::_narrow(object.in());

      is_ok = true;
      test_lock.acquire ();
      cond.signal();
      test_lock.release ();
      ACE_DEBUG ((LM_DEBUG, "SUCCESS: test did not spin.\n"));

      CORBA::String_var the_string = hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
