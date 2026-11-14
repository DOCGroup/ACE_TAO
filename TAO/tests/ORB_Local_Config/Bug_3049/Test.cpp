/// @author Phil Mesnier <mesnier_p@ociweb.com>

#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "Service_Configuration_Per_ORB.h"

constexpr ACE_TCHAR arg_a[] = ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBId ORB-A");
constexpr ACE_TCHAR arg_b[] = ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf a.conf");
constexpr ACE_TCHAR arg_c[] = ACE_TEXT ("BBB -ORBGestalt ORB:ORB-B -ORBId ORB-C");

/// Tests Bug 3049 across three ORBs: ORB-A is the default ORB and uses the
/// global configuration; ORB-B loads a service in its local configuration,
/// and ORB-C shares ORB-B's configuration. The service should be visible
/// through ORB-C but remain unavailable globally.
int
testBug3049 (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testBug3049");

  try
    {
      ACE_ARGV args_a (arg_a);
      int argc_a = args_a.argc ();
      CORBA::ORB_var const orb_a = CORBA::ORB_init (argc_a, args_a.argv ());

      ACE_ARGV args_b (arg_b);
      int argc_b = args_b.argc ();
      CORBA::ORB_var const orb_b = CORBA::ORB_init (argc_b, args_b.argv ());

      ACE_ARGV args_c (arg_c);
      int argc_c = args_c.argc ();
      CORBA::ORB_var const orb_c = CORBA::ORB_init (argc_c, args_c.argv ());

      // Confirm ORB-C inherited SHMIOP from ORB-B, while it is unavailable
      // in the global context.
      ACE_Service_Object const * const global_service =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("SHMIOP_Factory");
      bool failed {};
      if (global_service != nullptr)
        {
          failed = true;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Unexpected to find SHMIOP_Factory globally\n")));
        }

      ACE_Service_Object const * const orb_service =
        ACE_Dynamic_Service<ACE_Service_Object>::instance (
          orb_c->orb_core ()->configuration (), "SHMIOP_Factory");
      if (orb_service == nullptr)
        {
          failed = true;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Failed to find SHMIOP_Factory in ORB-C\n")));
        }

      orb_a->destroy ();
      orb_b->destroy ();
      orb_c->destroy ();

      if (failed)
        return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Unexpected exception\n")), -1);
    }

  return 0;
}

/// Runs the Bug 3049 test.
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testBug3049 (argc, argv);
}
