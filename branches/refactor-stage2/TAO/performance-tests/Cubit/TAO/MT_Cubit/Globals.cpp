// $Id$

#include "Globals.h"
#include "ace/Log_Msg.h"

Globals::Globals (void)
  : thr_create_flags (0),
    default_priority (0),
    ior_file (0),
    num_of_objs (2),
    thread_per_rate (0),
    use_multiple_priority (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    barrier_ (0)
{
  const char default_endpoint[] = "iiop://";
  // Default to iiop

  ACE_OS::strcpy (endpoint, default_endpoint);
}

int
Globals::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "e:t:f:rm");
  int c;

  while ((c = opts ()) != -1)
    {
      switch (c)
      {
      case 'm':
        use_multiple_priority = 1;
        break;
      case 'r':
        thread_per_rate = 1;
        break;
      case 'f':
        ACE_NEW_RETURN (ior_file,
                        char[BUFSIZ],
                        -1);
        ACE_OS::strcpy (ior_file,
                        opts.opt_arg ());
        break;
      case 'e':
        ACE_OS::strcpy (endpoint,
                        opts.opt_arg ());
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \t"
                           "[-e <endpoint>]                // starting endpoint                  \n\t\t\t"
                           "[-t <number_of_servants>]      // # of servant threads to create     \n\t\t\t"
                           "[-f <ior_file> ]               // specify a file to output all ior's \n\t\t\t"
                           "[-m ]                          // Use multiple priorities for threads\n\t\t\t"
                           "[-r ]                          // Run the thread-per-rate test       \n"
                           ,argv [0]),
                          -1);
      }
    }

  if (thread_per_rate == 1)
    num_of_objs = THREAD_PER_RATE_OBJS;

  // Indicates successful parsing of the command-line.
  return 0;
}

int
Globals::sched_fifo_init (void)
{
#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              SCHED_PRIORITY,
                                              ACE_SCOPE_PROCESS)))
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX,
                      "User is not superuser, "
                      "so remain in time-sharing class\n"));
          ACE_SET_BITS (GLOBALS::instance ()->thr_create_flags, THR_NEW_LWP);
          GLOBALS::instance ()->default_priority = ACE_THR_PRI_OTHER_DEF;
          return 1;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }

  ACE_SET_BITS (GLOBALS::instance ()->thr_create_flags, THR_BOUND);
  ACE_SET_BITS (GLOBALS::instance ()->thr_create_flags, THR_SCHED_FIFO);
  GLOBALS::instance ()->default_priority = ACE_THR_PRI_FIFO_DEF;

  return 0;
#else

  ACE_ERROR_RETURN ((LM_ERROR,
                     "Test will not run.  This platform doesn't seem to have threads.\n"),
                    -1);
#endif /* ACE_HAS_THREADS */
}

MT_Priority::MT_Priority (void)
  : num_priorities_ (0),
    grain_ (0)
{
}

ACE_Sched_Priority
MT_Priority::get_high_priority (void)
{
  ACE_Sched_Priority high_priority;

#if defined (VXWORKS)
  high_priority = GLOBALS::instance ()->default_priority;
#elif defined (ACE_WIN32)
  high_priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);
#else
  high_priority = GLOBALS::instance ()->default_priority;
#endif /* VXWORKS */
  return high_priority;
}

ACE_Sched_Priority
MT_Priority::get_low_priority (u_int num_low_priority,
                               ACE_Sched_Priority prev_priority,
                               u_int use_multiple_priority)
{
#if !defined (ACE_HAS_THREADS)
  ACE_UNUSED_ARG (num_low_priority);
  ACE_UNUSED_ARG (prev_priority);
  ACE_UNUSED_ARG (use_multiple_priority);
  return -1;
#else
  ACE_Sched_Priority low_priority = ACE_THR_PRI_FIFO_DEF;
  int policy = ACE_SCHED_FIFO;

  if (!ACE_BIT_ENABLED (GLOBALS::instance ()->thr_create_flags,
                        THR_SCHED_FIFO))
    {
      low_priority = ACE_THR_PRI_OTHER_DEF;
      policy = ACE_SCHED_OTHER;
    }

  // Drop the priority.
  if (use_multiple_priority)
    {
      this->num_priorities_ = 0;

      for (ACE_Sched_Priority_Iterator priority_iterator
             (policy, ACE_SCOPE_THREAD);
           priority_iterator.more ();
           priority_iterator.next ())
        this->num_priorities_++;

      // 1 priority is exclusive for the high priority client.
      this->num_priorities_--;

      // Drop the priority, so that the priority of clients will
      // increase with increasing client number.
      for (u_int j = 0;
           j < num_low_priority;
           j++)
        {
          low_priority =
            ACE_Sched_Params::previous_priority (policy,
                                                 prev_priority,
                                                 ACE_SCOPE_THREAD);
          prev_priority = low_priority;
        }
      // Granularity of the assignment of the priorities.  Some OSs
      // have fewer levels of priorities than we have threads in our
      // test, so with this mechanism we assign priorities to groups
      // of threads when there are more threads than priorities.
      this->grain_ = num_low_priority / this->num_priorities_;

      if (this->grain_ <= 0)
        this->grain_ = 1;
    }
  else
    low_priority =
      ACE_Sched_Params::previous_priority (policy,
                                           prev_priority,
                                           ACE_SCOPE_THREAD);
  return low_priority;
#endif /* ACE_HAS_THREADS */
}

u_int
MT_Priority::number_of_priorities (void)
{
  return this->num_priorities_;
}

u_int
MT_Priority::grain (void)
{
  return this->grain_;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
