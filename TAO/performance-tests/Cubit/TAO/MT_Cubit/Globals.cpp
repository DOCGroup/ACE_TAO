// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Globals.cpp
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
// ============================================================================

#include "Globals.h"

Globals::Globals (void)
  : ior_file (0),
    base_port (0),
    num_of_objs (2),
    use_name_service (1),
    thread_per_rate (0),
    use_multiple_priority (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    barrier_ (0)
{
  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    ACE_DEBUG ((LM_DEBUG,
                "%p\n",
                "gethostname"));
}

int
Globals::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "sh:p:t:f:rm");
  int c;

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              hostname));

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
      case 's':
        use_name_service = 0;
        break;
      case 'f':
        ACE_NEW_RETURN (ior_file,
                        char[BUFSIZ],
                        -1);
        ACE_OS::strcpy (ior_file,
                        opts.optarg);
        break;
      case 'h':
        ACE_OS::strcpy (hostname,
                        opts.optarg);
        break;
      case 'p':
        base_port = ACE_OS::atoi (opts.optarg);
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " \t[-s Means NOT to use the name service] \n"
                           " \t[-p <port>]\n"
                           " \t[-h <my_hostname>]\n"
                           " \t[-t <num_objects>]\n"
                           " \t[-f <ior_file>]\n"
                           " \t[-r Use thread per rate]\n"
                           "\n", argv [0]),
                          -1);
      }
    }

  if (thread_per_rate == 1)
    // @@ Naga, can you please make this a symbolic constant rather
    // than the magic number "4"?!
    num_of_objs = 4;

  // Indicates successful parsing of the command-line.
  return 0;
}

MT_Priority::MT_Priority (void)
  :num_priorities_ (0),
   grain_ (0)
{
}

ACE_Sched_Priority
MT_Priority::get_high_priority (void)
{
  ACE_Sched_Priority high_priority;

#if defined (VXWORKS)
  high_priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  high_priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);
#else
  // @@ Naga/Sergio, why is there a "25" here?  This seems like to
  // much of a "magic" number.  Can you make this more "abstract?"
  high_priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* VXWORKS */
  return high_priority;
}

ACE_Sched_Priority
MT_Priority::get_low_priority (u_int num_low_priority,
                               ACE_Sched_Priority prev_priority,
                               u_int use_multiple_priority)
{
  ACE_Sched_Priority low_priority = ACE_THR_PRI_FIFO_DEF;

  // Drop the priority.
  if (use_multiple_priority)
    {
      this->num_priorities_ = 0;

      for (ACE_Sched_Priority_Iterator priority_iterator 
             (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);
           priority_iterator.more ();
           priority_iterator.next ())
        this->num_priorities_ ++;

      // 1 priority is exclusive for the high priority client.
      this->num_priorities_ --;

      // Drop the priority, so that the priority of clients will
      // increase with increasing client number.
      for (u_int j = 0;
           j < num_low_priority;
           j++)
        {
          low_priority =
            ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
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
      ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                           prev_priority,
                                           ACE_SCOPE_THREAD);
  return low_priority;
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

