// $Id$

#include "ace/Get_Opt.h"
#include "ace/Thread.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#if defined (ACE_HAS_TRACE)
# include "ace/OS_NS_strings.h"
#endif /* ACE_HAS_TRACE */

#include "Options.h"



/* static */
Options *Options::instance_ = 0;

Options *
Options::instance (void)
{
  if (Options::instance_ == 0)
    Options::instance_ = new Options;

  return Options::instance_;
}

Options::Options (void)
  : thr_count_ (4),
    t_flags_ (0),
    high_water_mark_ (8 * 1024),
    low_water_mark_ (1024),
    message_size_ (128),
    initial_queue_length_ (0),
    iterations_ (100000),
    debugging_ (0),
    verbosity_ (0),
    consumer_port_ (ACE_DEFAULT_SERVER_PORT),
    supplier_port_ (ACE_DEFAULT_SERVER_PORT + 1)
{
}

Options::~Options (void)
{
}

void Options::print_results (void)
{
#if !defined (ACE_WIN32)
  ACE_Profile_Timer::ACE_Elapsed_Time et;

  this->itimer_.elapsed_time (et);

  if (this->verbose ())
    {
#if defined (ACE_HAS_PRUSAGE_T)
      ACE_Profile_Timer::Rusage rusage;
      this->itimer_.get_rusage (rusage);

      ACE_OS::printf ("final concurrency hint = %d\n", ACE_Thread::getconcurrency ());
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
                      (int) rusage.pr_lwpid,
                      (int) rusage.pr_count,
                      (int) rusage.pr_minf,
                      (int) rusage.pr_majf,
                      (int) rusage.pr_inblk,
                      (int) rusage.pr_oublk,
                      (int) rusage.pr_msnd,
                      (int) rusage.pr_mrcv,
                      (int) rusage.pr_sigs,
                      (int) rusage.pr_wtime.tv_sec, (int) rusage.pr_wtime.tv_nsec / 1000000,
                      (int) rusage.pr_ltime.tv_sec, (int) rusage.pr_ltime.tv_nsec / 1000000,
                      (int) rusage.pr_slptime.tv_sec, (int) rusage.pr_slptime.tv_nsec / 1000000,
                      (int) rusage.pr_vctx,
                      (int) rusage.pr_ictx,
                      (int) rusage.pr_sysc,
                      (int) rusage.pr_ioch);
#else
      /* Someone needs to write the corresponding dump for rusage... */
#endif /* ACE_HAS_PRUSAGE_T */
    }

  ACE_OS::printf ("---------------------\n"
                  "real time   = %.3f\n"
                  "user time   = %.3f\n"
                  "system time = %.3f\n"
                  "---------------------\n",
                  et.real_time, et.user_time, et.system_time);
#endif /* ACE_WIN32 */
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("c:bdH:i:L:l:M:ns:t:T:v"));
  int c;

  while ((c = get_opt ()) != EOF)
  //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
      {
      case 'b':
        this->t_flags (THR_BOUND);
        break;
      case 'c':
        this->consumer_port (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'd':
        this->debugging_ = 1;
        break;
      case 'H':
        this->high_water_mark (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'i':
        this->iterations (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'L':
        this->low_water_mark (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'l':
        this->initial_queue_length (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'M':
        this->message_size (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'n':
        this->t_flags (THR_NEW_LWP);
        break;
      case 's':
        this->supplier_port (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'T':
        #if defined (ACE_HAS_TRACE)
        if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT ("ON")) == 0)
        ACE_Trace::start_tracing ();
        else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT ("OFF")) == 0)
        ACE_Trace::stop_tracing ();
        #endif /* ACE_HAS_TRACE */
        break;
      case 't':
        this->thr_count (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      case 'v':
        this->verbosity_ = 1;
        break;
      default:
        ACE_OS::fprintf (stderr, "%s\n"
                              "\t[-b] (THR_BOUND)\n"
                              "\t[-c consumer port]\n"
                              "\t[-d] (enable debugging)\n"
                              "\t[-H high water mark]\n"
                              "\t[-i number of test iterations]\n"
                              "\t[-L low water mark]\n"
                              "\t[-M] message size\n"
                              "\t[-n] (THR_NEW_LWP)\n"
                              "\t[-q max queue size]\n"
                              "\t[-s supplier port]\n"
                              "\t[-t number of threads]\n"
                              "\t[-v] (verbose)\n",
                              ACE_TEXT_ALWAYS_CHAR (argv[0]));
        ACE_OS::exit (1);
        /* NOTREACHED */
        break;
      }

  if (this->verbose ())
    ACE_OS::printf ("%8d = initial concurrency hint\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = total iterations\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = thread count\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = low water mark\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = high water mark\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = message_size\n"
                    ACE_SIZE_T_FORMAT_SPECIFIER_ASCII " = initial queue length\n"
                    "%8d = THR_BOUND\n"
                    "%8d = THR_NEW_LWP\n",
                    ACE_Thread::getconcurrency (),
                    this->iterations (),
                    this->thr_count (),
                    this->low_water_mark (),
                    this->high_water_mark (),
                    this->message_size (),
                    this->initial_queue_length (),
                    (this->t_flags () & THR_BOUND) != 0,
                    (this->t_flags () & THR_NEW_LWP) != 0);
}
