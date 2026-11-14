#include <vector>

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int
test_multiple_orbs (const char *test_name,
                    int argc,
                    ACE_TCHAR *argv[],
                    int iterations,
                    bool rt_orb,
                    bool destroy)
{
  std::vector<CORBA::ORB_var> orbs (iterations);
  std::vector<RTCORBA::RTORB_var> rt_orbs (iterations);

  try
    {
      for (int i = 0;
           i < iterations;
           ++i)
        {
          char name[100];
          ACE_OS::sprintf (name, "%s %d", test_name, i);

          orbs[i] =
            CORBA::ORB_init (argc,
                             argv,
                             name);

          if (rt_orb)
            {
              CORBA::Object_var object =
                orbs[i]->resolve_initial_references ("RTORB");

              rt_orbs[i] =
                RTCORBA::RTORB::_narrow (object.in ());

              ACE_ASSERT (rt_orbs[i].in () != RTCORBA::RTORB::_nil ());
            }
        }

      if (destroy)
        {
          for (int i = 0;
               i < iterations;
               ++i)
            {
              orbs[i]->destroy ();
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception caught in ORB_init");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int const iterations = 5;
  bool rt_orb = false;
  bool destroy = false;

  int result =
    test_multiple_orbs ("non-RT ORBs, disable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = true;
  rt_orb = false;

  result =
    test_multiple_orbs ("non-RT ORBs, enable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = false;
  rt_orb = true;

  result =
    test_multiple_orbs ("RT ORBs, disable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = true;
  rt_orb = true;

  result =
    test_multiple_orbs ("RT ORBs, enable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  return result;
}
