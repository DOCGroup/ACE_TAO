// $Id$

#include "tao/corba.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Get_Opt.h"

int
test_multiple_orbs (const char *test_name,
                    int argc,
                    char *argv[],
                    int iterations,
                    int rt_orb,
                    int destroy)
{
  CORBA::ORB_var *orbs =
    new CORBA::ORB_var[iterations];

  RTCORBA::RTORB_var *rt_orbs =
    new RTCORBA::RTORB_var[iterations];

  ACE_TRY_NEW_ENV
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
                             name
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (rt_orb)
            {
              CORBA::Object_var object =
                orbs[i]->resolve_initial_references ("RTORB"
                                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              rt_orbs[i] =
                RTCORBA::RTORB::_narrow (object.in ()
                                         ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_ASSERT (rt_orbs[i].in () != RTCORBA::RTORB::_nil ());
            }
        }

      if (destroy)
        {
          for (int i = 0;
               i < iterations;
               ++i)
            {
              orbs[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in ORB_init");
      return -1;
    }
  ACE_ENDTRY;

  delete[] rt_orbs;
  delete[] orbs;

  return 0;
}

int
main (int argc, char *argv[])
{
  int iterations = 5;
  int rt_orb = 0;
  int destroy = 0;

  int result =
    test_multiple_orbs ("non-RT ORBs, disable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = 1;
  rt_orb = 0;

  result =
    test_multiple_orbs ("non-RT ORBs, enable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = 0;
  rt_orb = 1;

  result =
    test_multiple_orbs ("RT ORBs, disable destroy",
                        argc,
                        argv,
                        iterations,
                        rt_orb,
                        destroy);
  ACE_ASSERT (result == 0);

  destroy = 1;
  rt_orb = 1;

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
