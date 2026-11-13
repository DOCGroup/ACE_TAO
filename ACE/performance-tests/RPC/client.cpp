#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Throughput_Stats.h"

#include "ping.h"

#include <limits>
#include <memory>

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  char const *host {};
  int nsamples {10000};
  int c {};

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("h:i:"));

  while ((c = getopt ()) != -1)
    {
  //FUZZ: enable check_for_lack_ACE_OS
      switch (c)
        {
        case 'h':
          host = getopt.opt_arg ();
          break;

        case 'i':
          {
            ACE_TCHAR *endp {};
            long const parsed_samples =
              ACE_OS::strtol (getopt.opt_arg (), &endp, 10);
            if (endp == getopt.opt_arg ()
                || *endp != ACE_TEXT ('\0')
                || parsed_samples <= 0
                || parsed_samples > std::numeric_limits<int>::max ())
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Iteration count must be a positive integer\n"));
                return 1;
              }
            nsamples = static_cast<int> (parsed_samples);
          }
          break;
        }
    }

  if (host == nullptr)
    {
      ACE_DEBUG ((LM_DEBUG, "Usage: client -h host -i iterations\n"));
      return 1;
    }

  std::unique_ptr<CLIENT, void (*) (CLIENT *)> cl (
    clnt_create (host, PINGPROG, PINGVERS, "tcp"),
    clnt_destroy);

  if (!cl)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot create client handle\n"));
      return 1;
    }

  ACE_Throughput_Stats throughput;

  ACE_hrtime_t const test_start = ACE_OS::gethrtime ();
  for (int i = 0; i != nsamples; ++i)
    {
      ACE_hrtime_t const start = ACE_OS::gethrtime ();

      int p {};
      if (ping_1 (&p, cl.get ()) == nullptr)
        {
          ACE_DEBUG ((LM_DEBUG, "RPC ping failed\n"));
          return 1;
        }

      ACE_hrtime_t const end = ACE_OS::gethrtime ();

      throughput.sample (end - test_start,
                         end - start);
    }

  ACE_DEBUG ((LM_DEBUG, "Calibrating high resolution timer . . ."));
  ACE_High_Res_Timer::global_scale_factor_type const gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, " done\n"));

  throughput.dump_results ("Client", gsf);

  return 0;
}
