#include "ace/Get_Opt.h"
// $Id$

#include "ace/Synch.h"

#include "Options.h"

#if defined (ACE_HAS_THREADS)

Options::Options (void)
		 : thr_count_ (4),
		   t_flags_ (THR_DETACHED),
		   high_water_mark_ (8 * 1024),
		   low_water_mark_ (1024),
		   message_size_ (128),
		   initial_queue_length_ (0),
		   iterations_ (100000),
		   debugging_ (0),
		   verbosity_ (0),
		   consumer_port_ ("-p 10000"),
		   supplier_port_ ("-p 10001"),
		   consumer_file_ ("-f/tmp/conupipe"),
		   supplier_file_ ("-f/tmp/supupipe")
{
}

Options::~Options (void)
{
}

void Options::print_results (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  this->itimer_.elapsed_time (et);

#if defined (ACE_HAS_PRUSAGE_T)
  prusage_t rusage;
  this->itimer_.get_rusage (rusage);

  if (options.verbose ())
    {
      ACE_OS::printf ("final concurrency hint = %d\n", ACE_OS::thr_getconcurrency ());
      ACE_OS::printf ("%8d = lwpid\n"
		"%8d = lwp count\n"
		"%8d = minor page faults\n"
		"%8d = major page faults\n"
		"%8d = input blocks\n"
		"%8d = output blocks\n"
		"%8d = messages sent\n"
		"%8d = messages received\n"
		"%8d = signals received\n"
		"%8ds, %dms = wait-cpu (latency) time\n"		
		"%8ds, %dms = user lock wait sleep time\n"
		"%8ds, %dms = all other sleep time\n"
		"%8d = voluntary context switches\n"
		"%8d = involuntary context switches\n"
		"%8d = system calls\n"
		"%8d = chars read/written\n",
		rusage.pr_lwpid,
		rusage.pr_count,
		rusage.pr_minf,
		rusage.pr_majf,
		rusage.pr_inblk,
		rusage.pr_oublk,
		rusage.pr_msnd,
		rusage.pr_mrcv,
		rusage.pr_sigs,
		rusage.pr_wtime.tv_sec, rusage.pr_wtime.tv_nsec / 1000000,
		rusage.pr_ltime.tv_sec, rusage.pr_ltime.tv_nsec / 1000000,
		rusage.pr_slptime.tv_sec, rusage.pr_slptime.tv_nsec / 1000000,
		rusage.pr_vctx,
		rusage.pr_ictx,
		rusage.pr_sysc,
		rusage.pr_ioch);
    }
#endif /* ACE_HAS_PRUSAGE_T */

  ACE_OS::printf ("---------------------\n"
	    "real time   = %.3f\n"
	    "user time   = %.3f\n"
	    "system time = %.3f\n"
	    "---------------------\n",
	    et.real_time, et.user_time, et.system_time);
}

// Manages the options.
Options options;

void
Options::parse_args (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt getopt (argc, argv, "C:c:bdH:i:L:l:M:nS:s:t:T:v");
  int c;

  while ((c = getopt ()) != -1)
    switch (c)
      {
      case 'b':
	this->t_flags (THR_BOUND);
	break;
      case 'C':
	this->consumer_file (getopt.optarg);
	break;
      case 'c':
	this->consumer_port (getopt.optarg);
	break;
      case 'd':
	this->debugging_ = 1;
	break;
      case 'H':
	this->high_water_mark (ACE_OS::atoi (getopt.optarg));
	break;
      case 'i':
	this->iterations (ACE_OS::atoi (getopt.optarg));
	break;
      case 'L':
	this->low_water_mark (ACE_OS::atoi (getopt.optarg));
	break;
      case 'l':
	this->initial_queue_length (ACE_OS::atoi (getopt.optarg));
	break;
      case 'M':
	this->message_size (ACE_OS::atoi (getopt.optarg));
	break;
      case 'n':
	this->t_flags (THR_NEW_LWP);
	break;
      case 'S':
	this->supplier_file (getopt.optarg);
	break;
      case 's':
	this->supplier_port (getopt.optarg);
	break;
      case 'T':
	if (ACE_OS::strcasecmp (getopt.optarg, "ON") == 0)
	  ACE_Trace::start_tracing ();
	else if (ACE_OS::strcasecmp (getopt.optarg, "OFF") == 0)
	  ACE_Trace::stop_tracing ();
	break;
      case 't':
	this->thr_count (ACE_OS::atoi (getopt.optarg));
	break;
      case 'v':
	this->verbosity_ = 1;
	break;
      default:
	::fprintf (stderr, "%s\n"
		   "\t[-b] (THR_BOUND)\n"
		   "\t[-C consumer file]\n"
		   "\t[-c consumer port]\n"
		   "\t[-d] (enable debugging)\n"
		   "\t[-H high water mark]\n"
		   "\t[-i number of test iterations]\n"
		   "\t[-L low water mark]\n"
		   "\t[-M] message size \n" 
		   "\t[-n] (THR_NEW_LWP)\n" 
		   "\t[-q max queue size]\n"
		   "\t[-S supplier file]\n"
		   "\t[-s supplier port]\n"
		   "\t[-t number of threads]\n"
		   "\t[-v] (verbose) \n",
		   argv[0]);
	::exit (1);
	/* NOTREACHED */
	break;
      }

  if (this->verbose ())
    ACE_OS::printf ("%8d = initial concurrency hint\n"
	      "%8d = total iterations\n"
	      "%8d = thread count\n"
	      "%8d = low water mark\n"
	      "%8d = high water mark\n"
	      "%8d = message_size\n"
	      "%8d = initial queue length\n"
	      "%8d = THR_BOUND\n"
	      "%8d = THR_NEW_LWP\n",
	      ACE_OS::thr_getconcurrency (),
	      this->iterations (),
	      this->thr_count (),
	      this->low_water_mark (),
	      this->high_water_mark (),
	      this->message_size (),
	      this->initial_queue_length (),
	      (this->t_flags () & THR_BOUND) != 0,
	      (this->t_flags () & THR_NEW_LWP) != 0);
}

#endif /* ACE_HAS_THREADS */
