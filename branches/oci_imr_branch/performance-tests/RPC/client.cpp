/*
 * $Id$
 */
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"

#include "ping.h"

int main (int argc, char* argv[])
{
  const char* host = 0;
  int nsamples = 10000;
  int c;

  ACE_Get_Opt getopt (argc, argv, "h:i:");

  while ((c = getopt ()) != -1)
    {
      switch ((char) c)
        {
        case 'h':
          host = getopt.opt_arg ();
          break;

        case 'i':
          nsamples = ACE_OS::atoi (getopt.opt_arg ());
          break;
        }
    }

  if (host == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Usage: client -h host -i iterations\n"));
      return 1;
    }

  CLIENT *cl =
    clnt_create (host, PINGPROG, PINGVERS, "tcp");

  if (cl == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot create client handle\n"));
      return 1;
    }

  ACE_Throughput_Stats throughput;

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i != nsamples; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      int p = 0;
      (void) ping_1 (&p, cl);

      ACE_hrtime_t end = ACE_OS::gethrtime ();

      throughput.sample (end - test_start,
                         end - start);

    }

  ACE_DEBUG ((LM_DEBUG, "Calibrating high resolution timer . . ."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, " done\n"));

  throughput.dump_results ("Client", gsf);


  return 0;
}
