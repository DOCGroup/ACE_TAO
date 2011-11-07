// $Id$

#include "tao/corba.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"

// SSLIOP
const ACE_TCHAR argStrB[] = ACE_TEXT("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf b.conf");

// empty file
const ACE_TCHAR argStrM[] = ACE_TEXT("MMM -ORBGestalt LOCAL -ORBId ORB-M -ORBSvcConf m.conf");

int
ACE_TMAIN (int, ACE_TCHAR **)
{
  int error = 0;

  try
    {
      ACE_ARGV argM (argStrM);
      int n = argM.argc ();
      CORBA::ORB_var ORBM =
        CORBA::ORB_init (n, argM.argv ());

      if (CORBA::is_nil (ORBM.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);

      // SSLIOP
      ACE_ARGV argB (argStrB);
      n = argB.argc ();
      CORBA::ORB_var ORBB =
        CORBA::ORB_init (n, argB.argv ());

      if (CORBA::is_nil (ORBB.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);

      // Since each svc conf file causes the ORB to load the services in
      // its own service space no services are reachable through the
      // global service repo

      // SSLIOP
      ACE_Service_Object *ssliop_so =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("SSLIOP_Factory");
      if (ssliop_so != 0)
        {
          ++error;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT("Unexpected to find ")
                      ACE_TEXT("SSLIOP_Factory globally\n")));
        }

      ORBM->destroy();

      ORBB->destroy();
    }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }

  return error;
}
