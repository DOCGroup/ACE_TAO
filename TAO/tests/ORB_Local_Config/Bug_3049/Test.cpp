// @author Phil Mesnier <mesnier_p@ociweb.com>

// This test scenario is documented in bugzilla bug 3049. This test
// creates 3 ORBs. ORB-A becomes the default ORB, which shares the
// global config context, ORB-B initializes a local configuration
// context which loads a service. ORB-C finally shares the configuration
// from ORB-B. The demonstration is to show that a service is avialable
// to ORB-C from the configuration initialized by ORB-B.

#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "Service_Configuration_Per_ORB.h"

ACE_TCHAR const arg_a[] = ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBId ORB-A");
ACE_TCHAR const arg_b[] = ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf a.conf");
ACE_TCHAR const arg_c[] = ACE_TEXT ("BBB -ORBGestalt ORB:ORB-B -ORBId ORB-C");

int
testBug3049 (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testBug3049");

  try
    {
      ACE_ARGV args_a (arg_a);
      int argc_a = args_a.argc ();
      CORBA::ORB_var orb_a (CORBA::ORB_init (argc_a, args_a.argv ()));

      ACE_ARGV args_b (arg_b);
      int argc_b = args_b.argc ();
      CORBA::ORB_var orb_b (CORBA::ORB_init (argc_b, args_b.argv ()));

      ACE_ARGV args_c (arg_c);
      int argc_c = args_c.argc ();
      CORBA::ORB_var orb_c (CORBA::ORB_init (argc_c, args_c.argv ()));

      // Confirm ORB-C inherited SHMIOP from ORB-B, while it is unavailable
      // in the global context.
      ACE_Service_Object * const global_service =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("SHMIOP_Factory");
      bool failed {};
      if (global_service != nullptr)
        {
          failed = true;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Unexpected to find SHMIOP_Factory globally\n")));
        }

      ACE_Service_Object * const orb_service =
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

// @brief the main driver

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testBug3049 (argc, argv);
}
