// $Id$
// @author Phil Mesnier <mesnier_p@ociweb.com>

// This test scenario is documented in bugzilla bug 3049. This test
// creates 3 ORBs. ORB-A becomes the default ORB, which shares the
// global config context, ORB-B initializes a local configuration
// context which loads a service. ORB-C finally shares the configuration
// from ORB-B. The demonstration is to show that a service is avialable
// to ORB-C from the configuration initalized by ORB-B.

#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "Service_Configuration_Per_ORB.h"

const ACE_TCHAR argA[] = ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBId ORB-A");
const ACE_TCHAR argB[] = ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf a.conf");
const ACE_TCHAR argC[] = ACE_TEXT ("BBB -ORBGestalt ORB:ORB-B -ORBId ORB-C");

int
testBug3049 (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testBug3049");

  try
  {
    ACE_ARGV arg0(argA);
    int n = arg0.argc();
    CORBA::ORB_var ORBA = CORBA::ORB_init(n,arg0.argv());

    ACE_ARGV arg1(argB);
    n = arg1.argc();
    CORBA::ORB_var ORBB = CORBA::ORB_init(n,arg1.argv());

    ACE_ARGV arg2(argC);
    n = arg2.argc();
    CORBA::ORB_var ORBC = CORBA::ORB_init(n,arg2.argv());

    // Confirm that ORBC has SHMIOP, which it inherits from ORBB, and
    // that SHMIOP is not available in the global context

    ACE_Service_Object *so = 0;
    int error = 0;
    so = ACE_Dynamic_Service<ACE_Service_Object>::instance ("SHMIOP_Factory");
    if (so != 0)
      {
        error++;
        ACE_ERROR ((LM_DEBUG,
                    ACE_TEXT("Unexpected to find SHMIOP_Factory globally\n")));
      }

    so = ACE_Dynamic_Service<ACE_Service_Object>::instance
      (ORBC->orb_core()->configuration(), "SHMIOP_Factory");
    if (so == 0)
      {
        error++;
        ACE_ERROR ((LM_DEBUG,
                    ACE_TEXT("Failed to find ")
                    ACE_TEXT("SHMIOP_Factory in ORBC\n")));
      }

    ORBA->destroy();

    ORBB->destroy();

    ORBC->destroy();

    if (error > 0)
      return -1;

  }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }
  catch(...)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unexpected exception\n")), -1);
    }

  return 0;
}

// @brief the main driver

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testBug3049(argc, argv);
}
