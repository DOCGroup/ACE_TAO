// $Id$
// @author Jeff Mirwaisi <jeff_mirwaisi@yahoo.com>
// @author Iliyan Jeliazkov <iliyan2ociweb.com>

// the following is a simplification of the above problems (see
// bugzilla 1459) and the different scenarios minus the creation of
// the servant the service manager or threads even without the servant
// to demonstrate that the server is capable of responding (which in
// some cases it isnt) problems can already be seen in the multiple
// orb scenarios


#include "tao/corba.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "Service_Configuration_Per_ORB.h"

// Currently there is no way to test SSLIOP in this test due to a problem
// described in bug 3418.
//#define DO_1459_SSLIOP_TEST 1

// UIOP
const ACE_TCHAR argStrA[] = ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBId ORB-A -ORBSvcConf a.conf");

// SSLIOP
const ACE_TCHAR argStrB[] = ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf b.conf");

// UIPMC
const ACE_TCHAR argStrC[] = ACE_TEXT ("CCC -ORBGestalt LOCAL -ORBId ORB-C -ORBSvcConf m1.conf");

// DIOP
const ACE_TCHAR argStrD[] = ACE_TEXT ("DDD -ORBGestalt LOCAL -ORBId ORB-D -ORBSvcConf d.conf");

// empty file
const ACE_TCHAR argStrM[] = ACE_TEXT ("MMM -ORBGestalt LOCAL -ORBId ORB-M -ORBSvcConf m.conf");

int
testBug1459 (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testBug1459");

  try
    {
      int n, error = 0;

      ACE_ARGV argM (argStrM);
      n = argM.argc ();
      CORBA::ORB_var ORBM =
        CORBA::ORB_init (n, argM.argv ());

      if (CORBA::is_nil (ORBM.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);

#if TAO_HAS_UIOP == 1
      // UIOP
      ACE_ARGV argA (argStrA);
      n = argA.argc ();
      CORBA::ORB_var ORBA =
        CORBA::ORB_init (n, argA.argv ());

      if (CORBA::is_nil (ORBA.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);
#endif /* TAO_HAS_UIOP */

#if defined (DO_1459_SSLIOP_TEST)
      // SSLIOP
      ACE_ARGV argB (argStrB);
      n = argB.argc ();
      CORBA::ORB_var ORBB =
        CORBA::ORB_init (n, argB.argv ());

      if (CORBA::is_nil (ORBB.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);
#endif

      // MIOP
      ACE_ARGV argC (argStrC);
      n = argC.argc ();
      CORBA::ORB_var ORBC =
        CORBA::ORB_init (n, argC.argv ());

      if (CORBA::is_nil (ORBC.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);

      // DIOP
      ACE_ARGV argD (argStrD);
      n = argD.argc ();
      CORBA::ORB_var ORBD =
        CORBA::ORB_init (n, argD.argv ());

      if (CORBA::is_nil (ORBD.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Expected to get an ORB\n")),
                          -1);

      if (
// SSLIOP
#if defined (DO_1459_SSLIOP_TEST)
          ORBM.in () == ORBB.in () ||
#if TAO_HAS_UIOP == 1
          ORBA.in () == ORBB.in () ||
#endif
          ORBC.in () == ORBB.in () ||
          ORBD.in () == ORBB.in () ||
#endif
// UIOP
#if TAO_HAS_UIOP
          ORBM.in () == ORBA.in () ||
          ORBC.in () == ORBA.in () ||
          ORBD.in () == ORBA.in () ||
#endif
// MIOP
          ORBM.in () == ORBC.in () ||
          ORBD.in () == ORBC.in () ||
// DIOP
          ORBM.in () == ORBD.in ()
         )
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT("Unexpected to find the two ORBs are the same\n")),
                          -1);

      // Since each svc conf file causes the ORB to load the services in
      // its own service space no services are reachable through the
      // global service repo

#if TAO_HAS_UIOP == 1
      // UIOP
      ACE_Service_Object *uiop_so =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("UIOP_Factory");
      if (uiop_so != 0)
        {
          ++error;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT("Unexpected to find ")
                      ACE_TEXT("UIOP_Factory globally\n")));
        }
#endif /* TAO_HAS_UIOP */

#if defined (DO_1459_SSLIOP_TEST)
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
#endif

      // MIOP
      ACE_Service_Object *uipmc_so =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("UIPMC_Factory");
      if (uipmc_so != 0)
        {
          ++error;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT("Unexpected to find ")
                      ACE_TEXT("UIPMC_Factory globally\n")));
        }

      // DIOP
      ACE_Service_Object *diop_so =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("DIOP_Factory");
      if (diop_so != 0)
        {
          ++error;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT("Unexpected to find ")
                      ACE_TEXT("DIOP_Factory globally\n")));
        }

      ORBM->destroy();

#if TAO_HAS_UIOP == 1
      ORBA->destroy();
#endif /* TAO_HAS_UIOP */

#if defined (DO_1459_SSLIOP_TEST)
      ORBB->destroy();
#endif

      ORBC->destroy();

      ORBD->destroy();

      if (error > 0)
        return -1;
    }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }

  return 0;
}

// @brief the main driver

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testBug1459(argc, argv);
}
