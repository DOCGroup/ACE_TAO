// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Options.h"

#if defined (ACE_HAS_THREADS)
// Manages the options.
Options options;

size_t
Options::count (void)
{
  size_t total = 0;

  for (int i = 0; i < options.thr_wc_size; i++)
    {
      if (options.thr_work_count[i] != 0)
	{
	  if (options.verbose ())
	    ACE_DEBUG ((LM_DEBUG, "count[%d] = %d\n", i, options.thr_work_count[i]));
	  total += options.thr_work_count[i];
	}
    }

  return total;
}

void
Options::init (void)
{
  for (int i = 0; i < this->thr_wc_size; i++)
    this->thr_work_count[i] = 0;
}

Options::Options (void)
		 : thr_wc_size (10000),
		   _service_entry (0),
		   _mapped_file (0),
		   _pipe_addr (ACE_DEFAULT_RENDEZVOUS),
		   _sleep_time (100),
		   _n_lwps (0),
		   _thr_count (4),
		   _t_flags (0),
		   _high_water_mark (8 * 1024),
		   _low_water_mark (1024),
		   _msg_size (128),
		   _initial_queue_length (0),
		   _logical_connections (1),
		   _physical_connections (1),
		   _iterations (100000),
		   _generate (0),
		   _udp (0),
		   _debugging (0),
		   _verbosity (0),
		   _ack (1),
		   _checksum (1),
		   _xdr (1),
		   _free_memory (1),
		   _zero_copy (0),
		   _print_summary (0),
		   _consecutive_ports (1),
		   _eager_exit (0)
{
  this->thr_work_count = new int[this->thr_wc_size];
  this->init ();
}

void
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "a:A:bBc:C:dDe:F:g:H:i:L:l:M:m:n:Np:P:s:S:t:T:uvX:Z:");
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'a':
	this->_ack = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'A':
	this->pipe_addr (get_opt.optarg);
	break;
      case 'B':
	this->t_flags (THR_BOUND);
	break;
      case 'c':
	{
	  long connections = ACE_OS::atoi (get_opt.optarg);

	  if (connections < 0)
	    this->physical_connections (size_t (-connections));
	  else if (connections > 0)
	    this->logical_connections (size_t (connections));
	  else
	    ACE_DEBUG ((LM_WARNING, "warning, 0 connections!\n"));

	  break;
	}
      case 'C':
	this->_checksum = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'd':
	this->_debugging = 1;
	break;
      case 'D':
	this->t_flags (THR_DETACHED);
	break;
      case 'e':
	this->_eager_exit = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'F':
	this->_free_memory = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'g':
	this->_generate = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'H':
	this->high_water_mark (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'i':
	this->iterations (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'L':
	this->low_water_mark (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'l':
	this->initial_queue_length (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'M':
	this->msg_size (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'm':
	this->mapped_file (get_opt.optarg);
	break;
      case 'N':
	this->t_flags (THR_NEW_LWP);
	break;
      case 'n':
	this->n_lwps (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'p':
	this->_print_summary = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'P':
	this->consecutive_ports (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'S':
	this->service_entry (get_opt.optarg);
	break;
      case 's':
	this->sleep_time (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'T':
	if (ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0)
	  ACE_Trace::start_tracing ();
	else if (ACE_OS::strcasecmp (get_opt.optarg, "OFF") == 0)
	  ACE_Trace::stop_tracing ();
	break;
      case 't':
	this->thr_count (ACE_OS::atoi (get_opt.optarg));
	break;
      case 'u':
	this->_udp = 1;
	break;
      case 'v':
	this->_verbosity = 1;
	break;
      case 'X':
	this->_xdr = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      case 'Z':
	this->_zero_copy = ACE_OS::strcasecmp (get_opt.optarg, "ON") == 0;
	break;
      default:
	ACE_DEBUG ((LM_INFO,
		   "%s\n"
		   "\t[-a] (send acknowledgement)\n"
		   "\t[-A] address of pipe [%s]\n"
		   "\t[-B] (THR_BOUND)\n"
		   "\t[-c] + number of logical connections\n"
		   "\t[-c] - number of physical connections\n"
		   "\t[-C] (enable checksumming)\n"		      
		   "\t[-d] (enable debugging)\n"
		   "\t[-D] (THR_DETACHED)\n"
		   "\t[-e] (eager exit)\n"
		   "\t[-F] (free memory)\n"
		   "\t[-g] (generate data)\n"
		   "\t[-H] high water mark\n"
		   "\t[-i] number of test iterations [%d]\n"
		   "\t[-L] low water mark\n"
		   "\t[-m] mapped file\n"
		   "\t[-M] message size\n" 
		   "\t[-n] number of LWPs\n" 		      
		   "\t[-N] (THR_NEW_LWP)\n" 
		   "\t[-p] (print benchmark summary)\n"
		   "\t[-P] number of consecutive ports\n"
		   "\t[-s] sleep time\n"
		   "\t[-S] service entry\n"
		   "\t[-t] number of threads [%d]\n"
		   "\t[-T] (enable tracing)\n"
		   "\t[-u] (UDP) \n"
		   "\t[-v] (verbose) \n"
		   "\t[-X] (enable xdr conversion)\n"
		   "\t[-Z] (enable zero-copy driver)\n%a",
		   argv[0],
		   this->pipe_addr (),
		   this->iterations (),
		   this->thr_count (),
		   1));
	/* NOTREACHED */
	break;
      }

  if (this->do_print_summary ())
    ACE_DEBUG ((LM_INFO,
		  "%8d = total iterations\n"
		  "%8d = logical connections\n"
		  "%8d = physical connections\n"
		  "%8d = message_size\n"
		  "%8d = calculated checksum\n"
		  "%8d = perform xdr conversion\n"
		  "%8d = number of LWPs requested\n"
		  "%8d = number of LWPs used\n",
		  this->iterations (),
		  this->logical_connections (),
		  this->physical_connections (),
		  this->msg_size (),
		  this->do_checksum () != 0,
		  this->do_xdr() != 0,
		  this->n_lwps (),
	       ACE_Thread::getconcurrency ()));
  else if (this->verbose ())
    ACE_DEBUG ((LM_INFO,
		  "%8d = total iterations\n"
		  "%8d = logical connections\n"
		  "%8d = physical connections\n"
		  "%8d = thread count\n"
		  "%8d = low water mark\n"
		  "%8d = high water mark\n"
		  "%8d = message_size\n"
		  "%8d = initial queue length\n"
		  "%8d = consecutive ports\n"
		  "%8d = calculated checksum\n"
		  "%8d = perform xdr conversion\n"
		  "%8d = zero-copy driver\n"
		  "%8d = free dynamic memory\n"
		  "%8d = print summary only\n"
		  "%8d = eager exit\n"
		  "%8d = UDP\n"		  
		  "%8d = send ack\n"
		  "%8d = THR_DETACHED\n"
		  "%8d = THR_BOUND\n"
		  "%8d = THR_NEW_LWP\n"
		  "%8d = sleep time\n",
		  this->iterations (),
		  this->logical_connections (),
		  this->physical_connections (),
		  this->thr_count (),
		  this->low_water_mark (),
		  this->high_water_mark (),
		  this->msg_size (),
		  this->initial_queue_length (),
		  this->consecutive_ports (),
		  this->do_checksum () != 0,
		  this->do_xdr() != 0,
		  this->do_zero_copy () != 0,
		  this->do_delete () != 0,
		  this->do_print_summary () != 0,
		  this->do_eager_exit () != 0,
		  this->do_udp () != 0,
		  this->do_ack () != 0,
		  (this->t_flags () & THR_DETACHED) != 0,
		  (this->t_flags () & THR_BOUND) != 0,
		  (this->t_flags () & THR_NEW_LWP) != 0,
	       this->sleep_time ()));
}

void
Options::print_results (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  this->_itimer.elapsed_time (et);

#if defined (ACE_HAS_PRUSAGE_T)
  ACE_Profile_Timer::Rusage rusage;
  this->_itimer.elapsed_rusage (rusage);
#endif /* ACE_HAS_PRUSAGE_T */

  size_t total    = this->count ();
  double nbytes   = total * this->msg_size ();
  double cpu_time = et.user_time + et.system_time;

#if 0
  mutex_timer.print_total ("ACE_Thread_Mutex overhead:", mutex_counter, 2);
  condition_timer.print_total ("ACE_Condition overhead:", condition_counter, 2);
  ACE_DEBUG ((LM_INFO,
		"%8d (number of ACE_Thread_Mutex operations)\n"
		"%8d (number of ACE_Condition operations)",
	     mutex_counter, condition_counter));
#endif /* NDEBUG */

  if (this->do_print_summary ())
    {
#if defined (ACE_HAS_PRUSAGE_T)
      ACE_DEBUG ((LM_INFO,
		    "\n%8d PEs\n"
		    "%8.2f Mbit/sec\n"
		    "%8d (voluntary context switches)\n"
		    "%8d (involuntary context switches)\n"
		    "%8d (total context switches)\n"		    
		    "%8d.%d sec (wait-cpu time)\n"		
		    "%8d.%d sec (user lock wait sleep time)\n"
		    "%8d.%d sec (all other sleep time)\n"
		    "%8d (major page faults)\n"
		    "%8d (minor page faults)\n"
		    "%8d (number of LWPs)\n",
		    this->thr_count (),
		    (nbytes / et.real_time) * 8.0 / 1024.0 / 1024.0,
		    rusage.pr_vctx,
		    rusage.pr_ictx,
		    rusage.pr_vctx + rusage.pr_ictx,
		    rusage.pr_wtime.tv_sec, rusage.pr_wtime.tv_nsec / 1000000,
		    rusage.pr_ltime.tv_sec, rusage.pr_ltime.tv_nsec / 1000000,
		    rusage.pr_slptime.tv_sec, rusage.pr_slptime.tv_nsec / 1000000,
		    rusage.pr_majf,
		    rusage.pr_minf,
		 ACE_Thread::getconcurrency ()));
#else
      // need to write dump ops for rusage...
#endif /* ACE_HAS_PRUSAGE_T */
    }
  else
    {
      ACE_DEBUG ((LM_INFO,
		    "\ntotal work = %d\n"
		    "(Only interpret the next two statistics for throughput tests)\n"
		    "%f bytes in %.2f real seconds = %.2f Mbit/sec\n"
		    "%f bytes in %.2f CPU seconds = %.2f Mbit/sec\n",
		    total,
		    nbytes, et.real_time, (nbytes / et.real_time) * 8.0 / 1024.0 / 1024.0,
		 nbytes, cpu_time, (nbytes / cpu_time) * 8.0 / 1024.0 / 1024.0));

#if defined (ACE_HAS_PRUSAGE_T)
      ACE_DEBUG ((LM_INFO,
		    "%8d = lwpid\n"
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
		    "%8d = total context switches\n"
		    "%8d = system calls\n"
		    "%8d = chars read/written\n"
		    "%8d = number of LWPs\n"
		    "---------------------\n"
		    "real time   = %.3f\n"
		    "user time   = %.3f\n"
		    "system time = %.3f\n"
		    "---------------------\n",
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
		    rusage.pr_vctx + rusage.pr_ictx,
		    rusage.pr_sysc,
		    rusage.pr_ioch,
		    ACE_Thread::getconcurrency (),
		 et.real_time, et.user_time, et.system_time));
#else
      // need to write dump ops for rusage...
#endif /* ACE_HAS_PRUSAGE_T */
    }
  if (options.do_eager_exit ())
    ACE_OS::_exit (0);
}
#endif /* ACE_HAS_THREADS */


#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if defined (ACE_MT_SAFE)
template class ACE_Atomic_Op<ACE_Thread_Mutex, size_t>;
#endif /* ACE_MT_SAFE */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

