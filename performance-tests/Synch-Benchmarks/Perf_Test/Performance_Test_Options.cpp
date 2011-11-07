// $Id$

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "ace/OS_NS_strings.h"

#if defined (ACE_HAS_THREADS)
// Manages the options.
Performance_Test_Options performance_test_options;

size_t
Performance_Test_Options::count (void)
{
  size_t total = 0;

  if (performance_test_options.verbose ())
    ACE_DEBUG ((LM_DEBUG, "Thread work count size = %d\n", performance_test_options.thr_wc_size));

  for (int i = 0; i < performance_test_options.thr_wc_size; i++)
    {
      if (performance_test_options.thr_work_count[i] != 0)
        {
          //          if (performance_test_options.verbose ())
            ACE_DEBUG ((LM_DEBUG, "count[%d] = %d\n", i, performance_test_options.thr_work_count[i]));
          total += performance_test_options.thr_work_count[i];
        }
    }

  return total;
}

void
Performance_Test_Options::init (void)
{
  for (int i = 0; i < this->thr_wc_size; i++)
    this->thr_work_count[i] = 0;
}

Performance_Test_Options::Performance_Test_Options (void)
                 : thr_wc_size (10000),
                   _service_entry (0),
                   _mapped_file (0),
                   _pipe_addr (const_cast<ACE_TCHAR *> (ACE_DEFAULT_RENDEZVOUS)),
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
Performance_Test_Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("a:A:bBc:C:dDe:F:g:H:i:L:l:M:m:n:Np:P:s:S:t:T:uvX:Z:"), 0);
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'a':                 // Not used. (do_ack ???)
        this->_ack = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'A':                 // Not used. (set rendezvous point.)
        this->pipe_addr (get_opt.opt_arg ());
        break;
      case 'B':                 // Create thread with THR_BOUND.
        this->t_flags (THR_BOUND);
        break;
      case 'c':                 // Not used. (# of connections.)
        {
          long connections = ACE_OS::atoi (get_opt.opt_arg ());

          if (connections < 0)
            this->physical_connections (size_t (-connections));
          else if (connections > 0)
            this->logical_connections (size_t (connections));
          else
            ACE_DEBUG ((LM_WARNING, "warning, 0 connections!\n"));

          break;
        }
      case 'C':                 // Not used.  (Toggle calculate checksum.)
        this->_checksum = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'd':                 // Not used.  (Enable debugging.)
        this->_debugging = 1;
        break;
      case 'D':                 // Create thread with THR_DETACHED.
        this->t_flags (THR_DETACHED);
        break;
      case 'e':                 // Perform eager exit (without cleaning up.)
        this->_eager_exit = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'F':                 // Not used.
        this->_free_memory = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'g':                 // Not used.  (generate data ??)
        this->_generate = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'H':                 // Not used.  (set high water mark)
        this->high_water_mark (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'i':                 // Not used.  (# of iterations)
        this->iterations (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'L':                 // Not used.  (set low water mark)
        this->low_water_mark (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'l':                 // Not used.  (set initial queue length)
        this->initial_queue_length (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'M':                 // Set message size in pipe_[proc|thr]_test.
        this->msg_size (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'm':                 // Not used.  (set mapped file name)
        this->mapped_file (get_opt.opt_arg ());
        break;
      case 'N':                 // Create thread with flag THR_NEW_LWP.
        this->t_flags (THR_NEW_LWP);
        break;
      case 'n':                 // Set # of lwp's
        this->n_lwps (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'p':                 // Toggle whether summary is printed.
        this->_print_summary = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'P':                 // Not used.
        this->consecutive_ports (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'S':                 // Not used  (set service_entry ???)
        this->service_entry (get_opt.opt_arg ());
        break;
      case 's':                 // Set testing duration.
        this->sleep_time (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'T':                 // Enable/disable tracing.
#if defined (ACE_HAS_TRACE)
        if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0)
          ACE_Trace::start_tracing ();
        else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("OFF")) == 0)
          ACE_Trace::stop_tracing ();
#endif /* ACE_HAS_TRACE */
        break;
      case 't':                 // Set # of threads contending the lock.
        this->thr_count (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'u':                 // Not used.  (use udp.)
        this->_udp = 1;
        break;
      case 'v':                 // Not used.  (set display verbosely)
        this->_verbosity = 1;
        break;
      case 'X':                 // Not used.  (Use xdr conversion.)
        this->_xdr = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
        break;
      case 'Z':                 // Not used.  (Do zero copy.)
        this->_zero_copy = ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("ON")) == 0;
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
                   "\t[-u] (UDP)\n"
                   "\t[-v] (verbose)\n"
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
Performance_Test_Options::print_results (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  this->_itimer.elapsed_time (et);

  ACE_Profile_Timer::Rusage rusage;
  this->_itimer.elapsed_rusage (rusage);

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
#elif defined (ACE_HAS_GETRUSAGE) && !defined (ACE_WIN32)
      ACE_DEBUG ((LM_INFO,
                  "\n%8d PEs\n"
                  "%8.2f Mbit/sec\n"
                  "%8d (voluntary context switches)\n"
                  "%8d (involuntary context switches)\n"
                  "%8d (total context switches)\n"
                  "%8d.%d sec (user time)\n"
                  "%8d.%d sec (system time)\n"
                  "%8d (major page faults)\n"
                  "%8d (minor page faults)\n"
                  "%8d (number of LWPs)\n",
                  this->thr_count (),
                  (nbytes / et.real_time) * 8.0 / 1024.0 / 1024.0,
                  rusage.ru_nvcsw,
                  rusage.ru_nivcsw,
                  rusage.ru_nvcsw + rusage.ru_nivcsw,
                  rusage.ru_utime.tv_sec, rusage.ru_utime.tv_usec / 1000000,
                  rusage.ru_stime.tv_sec, rusage.ru_stime.tv_usec / 1000000,
                  rusage.ru_majflt,
                  rusage.ru_minflt,
                  ACE_Thread::getconcurrency ()));
#elif defined (ACE_HAS_GETRUSAGE) && defined (ACE_WIN32)
      // Need more stuff for Win32.
      ACE_DEBUG ((LM_INFO,
                  "\n%8d PEs\n"
                  "%8.2f Mbit/sec\n"
                  "%8d (number of LWPs)\n",
                  this->thr_count (),
                  (nbytes / et.real_time) * 8.0 / 1024.0 / 1024.0,
                  ACE_Thread::getconcurrency ()));
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
#elif defined (ACE_HAS_GETRUSAGE) && !defined (ACE_WIN32)
      ACE_DEBUG ((LM_INFO,
                  "%8d = minor page faults\n"
                  "%8d = major page faults\n"
                  "%8d = input blocks\n"
                  "%8d = output blocks\n"
                  "%8d = messages sent\n"
                  "%8d = messages received\n"
                  "%8d = signals received\n"
                  "%8d = voluntary context switches\n"
                  "%8d = involuntary context switches\n"
                  "%8d = total context switches\n"
                  "%8d = number of LWPs\n"
                  "---------------------\n"
                  "real time   = %.3f\n"
                  "user time   = %.3f\n"
                  "system time = %.3f\n"
                  "---------------------\n",
                  rusage.ru_minflt,
                  rusage.ru_majflt,
                  rusage.ru_inblock,
                  rusage.ru_oublock,
                  rusage.ru_msgsnd,
                  rusage.ru_msgrcv,
                  rusage.ru_nsignals,
                  rusage.ru_nvcsw,
                  rusage.ru_nivcsw,
                  rusage.ru_nvcsw + rusage.ru_nivcsw,
                  ACE_Thread::getconcurrency (),
                  et.real_time, et.user_time, et.system_time));
#elif defined (ACE_HAS_GETRUSAGE) && defined (ACE_WIN32)
      // need to write more dump ops for rusage on Win32
      ACE_DEBUG ((LM_INFO,
                  "%8d = number of LWPs\n"
                  "---------------------\n"
                  "real time   = %.3f\n"
                  "user time   = %.3f\n"
                  "system time = %.3f\n"
                  "---------------------\n",
                  ACE_Thread::getconcurrency (),
                  et.real_time, et.user_time, et.system_time));
#endif /* ACE_HAS_PRUSAGE_T */
    }
  if (performance_test_options.do_eager_exit ())
    ACE_OS::_exit (0);
}
#endif /* ACE_HAS_THREADS */

