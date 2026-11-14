#include "tao/corba.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"

int
ACE_TMAIN (int, ACE_TCHAR **)
{
  int error {};

  try
    {
      // The empty service configuration must not expose SSLIOP globally.
      ACE_TCHAR const arg_str_m[] = ACE_TEXT ("MMM -ORBGestalt LOCAL -ORBId ORB-M -ORBSvcConf m.conf");
      ACE_ARGV arg_m (arg_str_m);
      int argc_m = arg_m.argc ();
      CORBA::ORB_var orb_m = CORBA::ORB_init (argc_m, arg_m.argv ());

      if (CORBA::is_nil (orb_m.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("Expected to get an ORB\n")),
                          -1);

      // This service configuration loads SSLIOP only into this ORB's service space.
      ACE_TCHAR const arg_str_b[] = ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBId ORB-B -ORBSvcConf b.conf");
      ACE_ARGV arg_b (arg_str_b);
      int argc_b = arg_b.argc ();
      CORBA::ORB_var orb_b = CORBA::ORB_init (argc_b, arg_b.argv ());

      if (CORBA::is_nil (orb_b.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("Expected to get an ORB\n")),
                          -1);

      // Since each svc conf file causes the ORB to load the services in
      // its own service space no services are reachable through the
      // global service repo

      // SSLIOP
      ACE_Service_Object *const ssliop_so =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("SSLIOP_Factory");
      if (ssliop_so != nullptr)
        {
          ++error;
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Unexpected to find ")
                      ACE_TEXT ("SSLIOP_Factory globally\n")));
        }

      orb_m->destroy ();

      orb_b->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }

  return error;
}
