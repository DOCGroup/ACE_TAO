// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler_Generic.cpp
//
// = CREATION DATE
//    19 November 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#include "ace/Sched_Params.h"

#include "Scheduler_Generic.h"

#if ! defined (__ACE_INLINE__)
#include "Scheduler_Generic.i"
#endif /* __ACE_INLINE__ */


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// static functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Structure for storing the RT_Info information for each task, per mode.
struct Mode_Entry
{
  RtecScheduler::RT_Info *rt_info_;
  u_long start_time_; // microseconds
  u_long stop_time_;  // microseconds

  Mode_Entry() :
    rt_info_ (0),
    start_time_ (0),
    stop_time_ (0)
  {
  }

  Mode_Entry(RtecScheduler::RT_Info *const rt_info,
             const u_long start_time = 0,
             const u_long stop_time = 0) :
    rt_info_ (rt_info),
    start_time_ (start_time),
    stop_time_ (stop_time)
  {
  }

  ~Mode_Entry () {}

  Mode_Entry &operator= (const Mode_Entry &entry)
  {
    if (this != &entry)
      {
        rt_info_ = entry.rt_info_;
        start_time_ = entry.start_time_;
        stop_time_ = entry.stop_time_;
      }

    return *this;
  }
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Scheduler_Generic member functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Scheduler_Generic::Scheduler_Generic () :
  Scheduler (),
  handles_ (0),
  // Set the minimum priority to that for the current platform.  This
  // shouldn't be necessary, but UPSingleProcessorOrb::initialize_reactors
  // creates threads before the Event Channel calls Scheduler::init ().
  minimum_priority_ (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                     ACE_SCOPE_THREAD)),
  increasing_priority_ (-1),
  task_entries_ ()
{
}


Scheduler_Generic::~Scheduler_Generic ()
{
  reset ();
}


void
Scheduler_Generic::reset ()
{
}

Scheduler::status_t
Scheduler_Generic::lookup_rt_info (handle_t handle,
                                   RT_Info*& rtinfo)
{
  if (handle < 0 || (size_t) handle > task_entries_.size ())
    {
      return ST_UNKNOWN_TASK;
    }
  RT_Info*** entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> i (task_entries_);
  while (i.next (entry) != 0)
    {
      i.advance ();
      RT_Info** array = *entry;
      if (array[0]->handle == handle)
        {
          rtinfo = array[0];
          return SUCCEEDED;
        }
    }

  return ST_UNKNOWN_TASK;
}


Scheduler::status_t
Scheduler_Generic::register_task (RT_Info *rt_info [],
                                   const u_int number_of_modes,
                                   handle_t &handle)
{
  status_t ret;

  // try to store the new task's information . . .
  switch (task_entries_.insert (rt_info))
    {
      case 0 : // successfully inserted
        {
          rt_info [0]->handle = (handle = ++handles_);

          // assigned the same handle to the RT_Info for each of its modes
          for (u_int i = 1; i < number_of_modes; ++i)
            {
              if (rt_info [i] != 0)
                rt_info [i]->handle = handle;
            }

          if (number_of_modes > modes ())
            {
              modes (number_of_modes);
            }

          ret = SUCCEEDED;

          if (output_level () >= 5)
            {
              ACE_OS::printf ("registered task \"%s\" with RT_Info starting "
                                "at %X\n",
                              (const char*)rt_info[0]->entry_point,
                              (void *) rt_info[0]);
            }
        }
        break;

      case 1 : // the entry had already been inserted
        handle = 0;
        ret = ST_TASK_ALREADY_REGISTERED;
        break;

      default :
        // case -1 : insert failed, probably because virtual memory exhaused
        handle = 0;
        ret = ST_VIRTUAL_MEMORY_EXHAUSTED;
        break;
    }

  return ret;
}


void
Scheduler_Generic::init (const int minimum_priority,
                          const int maximum_priority,
                          const char *runtime_filename,
                          const char *rt_info_filename,
                          const char *timeline_filename)
{
  minimum_priority_ = minimum_priority;
  maximum_priority_ = maximum_priority;
  runtime_filename_ = runtime_filename;
  rt_info_filename_ = rt_info_filename;
  timeline_filename_ = timeline_filename;
}


Scheduler::status_t
Scheduler_Generic::schedule (void)
{
  ACE_Guard<LOCK> ace_mon (lock_);

  // here goes . . .

  increasing_priority_ = maximum_priority_ >= minimum_priority_;

  status_t status = NOT_SCHEDULED;

  // store number of tasks, based on registrations
  tasks (task_entries_.size ());

  if (output_level () > 0)
    {
      print_schedule ();
    }

  if (runtime_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
    {
      status = store_schedule (runtime_filename_);
    }

  if ((status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED)  &&  rt_info_filename_)
    {
      status = store_rt_info (rt_info_filename_);
    }

  // If there was a failure, (try to) remove the output files.
  if (! (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
    {
      if (runtime_filename_ && unlink ((char *) runtime_filename_)
          &&  errno != ENOENT)
        {
          ACE_OS::perror ("Scheduler_Generic::schedule (); "
                          "unable to remove schedule file");
        }
      if (rt_info_filename_  &&  unlink ((char *) rt_info_filename_)  &&
          errno != ENOENT)
        {
          ACE_OS::perror ("Scheduler_Generic::schedule (); "
                          "unable to remove rt_info file");
        }
    }

  return status;
}


int
Scheduler_Generic::priority (const handle_t handle,
                             OS_Thread_Priority &priority,
                             Sub_Priority &subpriority,
                             Preemption_Priority &preemption_prio,
                             const mode_t requested_mode) const
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (requested_mode);

  priority = minimum_priority_;
  subpriority = ACE_Scheduler_MIN_SUB_PRIORITY;
  preemption_prio = ACE_Scheduler_MAX_PREEMPTION_PRIORITY;

  if (output_level () >= 3)
    {
      ACE_OS::printf ("preemption_prio %d: min %d, pri %d, min_pri %d\n",
                      preemption_prio, minimum_priority_queue (),
                      priority, minimum_priority_);
    }

  return 0;
}


void
Scheduler_Generic::print_schedule ()
{
}


Scheduler::status_t
Scheduler_Generic::store_schedule (const char *filename)
{
  u_int i;
  RT_Info ***entry;
  FILE *const file = ACE_OS::fopen (filename, "w");

  if (file)
    {
      ACE_OS::fprintf (file, "\
// Automatically generated \"%s\"\n\
\n\
#include \"Scheduler_Runtime.h\"\n\
\n\
static const unsigned int MODES = %u;\n\
static const unsigned int TASKS = %u;\n\
static const unsigned int THREADS = %u;\n\
static const unsigned int MINIMUM_PRIORITY_QUEUE = %u;\n\
\n\
int\n\
Scheduler_Runtime_registered_tasks_ [TASKS] = { 0 };\n\
\n\
const char *\n\
Scheduler_Runtime_task_names_ [TASKS] =\n\
  {\n\
",                     filename,
                       modes (),
                       tasks (),
                       threads (),
                       minimum_priority_queue ());

      ACE_Unbounded_Set_Iterator <RT_Info **>
        task_entries_i (task_entries_);
      i = 0;
      while (task_entries_i.next (entry) != 0)
        {
          task_entries_i.advance ();
          ACE_OS::fprintf (file, "    \"%s\"%s\n",
                           (const char*) (*entry) [0]->entry_point,
                           ++i == tasks ()  ?  ""  :  ",");
        }

      ACE_OS::fprintf (file, "\
  };\n\
\n\
int\n\
Scheduler_Runtime_priorities_ [MODES][TASKS][3] =\n\
  {\n\
");

      for (i = 0; i < modes (); ++i)
        {
          ACE_OS::fprintf (file, "    { /* mode %u */\n", i);

          ACE_Unbounded_Set_Iterator <RT_Info **>
            task_entries_i (task_entries_);
          u_int j = 0;
          while (task_entries_i.next (entry) != 0)
            {
              task_entries_i.advance ();

              RtecScheduler::OS_Priority priority;
              RtecScheduler::Sub_Priority subpriority;
              RtecScheduler::Preemption_Priority preemption_prio;
              this->priority (0, priority, subpriority, preemption_prio, j);

              ACE_OS::fprintf (file, "    \"%s\"%s\n",
                               (const char*) (*entry) [0]->entry_point,
                               j + 1 == tasks ()  ?  ""  :  ",");

              ACE_OS::fprintf (file, "      { %d, %d, %u }%s\n",
                               priority,
                               subpriority,
                               preemption_prio,
                               ++j == tasks ()  ?  ""  :  ",");
            }
          ACE_OS::fprintf (file, "    }%s /* end mode %u */\n",
                           i == modes () - 1  ?  ""  :  ",", i);
        }

      ACE_OS::fprintf (file, "\
  };\n\
\n\
unsigned int\n\
Scheduler_Runtime::modes ()\n\
{\n\
  return MODES;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::tasks ()\n\
{\n\
  return TASKS;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::threads ()\n\
{\n\
  return THREADS;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::minimum_priority_queue ()\n\
{\n\
  return MINIMUM_PRIORITY_QUEUE;\n\
}\n\
\n\
const char *\n\
Scheduler_Runtime::task_name (const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_task_names_ [handle - 1];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::priority (const unsigned int mode,\n\
                                        const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][0];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::subpriority (const unsigned int mode,\n\
                                           const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][1];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::preemption_prio (const unsigned int mode,\n\
                                            const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][2];\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::config ()\n\
{\n\
  return 0;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::find (const char *operation_name)\n\
{\n\
  for (unsigned int i = 0; i < TASKS; ++i)\n\
    if (! ACE_OS::strcmp (operation_name,\n\
                          Scheduler_Runtime_task_names_ [i]))\n\
      return i + 1;\n\
\n\
  return 0;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::register_task (const unsigned int task)\n\
{\n\
  if (Scheduler_Runtime_registered_tasks_ [task - 1] == 1)\n\
    {\n\
      return 0;\n\
    }\n\
  else\n\
    {\n\
      Scheduler_Runtime_registered_tasks_ [task - 1] = 1;\n\
      return task;\n\
    }\n\
}\n\
\n\
");

      if (ACE_OS::fprintf (file, "// EOF\n") > 0  &&
          ACE_OS::fclose (file) == 0)
        {
          return SUCCEEDED;
        }
      else
        {
          return UNABLE_TO_WRITE_SCHEDULE_FILE;
        }
    }
  else
    {
      return UNABLE_TO_OPEN_SCHEDULE_FILE;
    }
}


Scheduler::status_t
Scheduler_Generic::store_rt_info (const char *filename)
{
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
    {
      const time_t now = ACE_OS::time (0);

      (void) ACE_OS::fprintf (file,
                              "# RT_Info provided for \"%s\" %s"
                              "# Version 1.1\n"
                              "# Format for each entry:\n"
                              "#   entry name\n"
                              "#   handle\n"
                              "#   worst case execution time\n"
                              "#   typical execution time\n"
                              "#   cached execution time\n"
                              "#   period\n"
                              "#   importance\n"
                              "#   quantum\n"
                              "# begin dependencies\n"
                              "#   number of dependencies\n"
                              "#   entry name, number of calls "
                                "(one of these lines per dependency, if any)\n"
                              "# end dependencies\n"
                              "#   priority\n"
                              "#   order within priority\n\n"
                              "%u modes\n%u operations\n\n",
                              filename, ACE_OS::ctime (&now),
                              modes (), tasks ());

      RT_Info ***entry;
      ACE_Unbounded_Set_Iterator <RT_Info **>
        task_entries_i (task_entries_);
      while (task_entries_i.next (entry) != 0)
        {
          task_entries_i.advance ();
          export ((*entry) [0], file);
        }
    }
  else
    {
      return UNABLE_TO_OPEN_SCHEDULE_FILE;
    }

  if (ACE_OS::fprintf (file, "\n# end of file\n", 1) > 0  &&
      ACE_OS::fclose (file) == 0)
    {
      return SUCCEEDED;
    }
  else
    {
      return UNABLE_TO_WRITE_SCHEDULE_FILE;
    }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<RtecScheduler::RT_Info **>;
template class ACE_Unbounded_Set<RtecScheduler::RT_Info **>;
template class ACE_Unbounded_Set_Iterator<RtecScheduler::RT_Info **>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


// EOF
