// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler.cpp
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#include "ace/Sched_Params.h"
#include "orbsvcs/Time_Utilities.h"
#include "Scheduler.h"

#if ! defined (__ACE_INLINE__)
#include "Scheduler.i"
#endif /* __ACE_INLINE__ */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Scheduler static members
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const ACE_Scheduler::mode_t ACE_Scheduler::CURRENT_MODE = 0xFFFFFFFF;

ACE_Scheduler *ACE_Scheduler::instance_ = 0;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class ACE_Scheduler static functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void
ACE_Scheduler::output (FILE *file, const status_t status)
{
  switch (status)
  {
    case NOT_SCHEDULED :
      ACE_OS::fprintf (file, "NOT_SCHEDULED");
      break;
    case SUCCEEDED :
      ACE_OS::fprintf (file, "SUCCEEDED");
      break;
    case ST_TASK_ALREADY_REGISTERED :
      ACE_OS::fprintf (file, "TASK_ALREADY_REGISTERED");
      break;
    case ST_VIRTUAL_MEMORY_EXHAUSTED :
      ACE_OS::fprintf (file, "VIRTUAL_MEMORY_EXHAUSTED");
      break;
    case ST_UNKNOWN_TASK :
      ACE_OS::fprintf (file, "UNKNOWN_TASK");
      break;
    case INVALID_MODE :
      ACE_OS::fprintf (file, "INVALID_MODE");
      break;
    case MODE_COUNT_MISMATCH :
      ACE_OS::fprintf (file, "MODE_COUNT_MISMATCH");
      break;
    case TASK_COUNT_MISMATCH :
      ACE_OS::fprintf (file, "TASK_COUNT_MISMATCH");
      break;
    case INVALID_PRIORITY :
      ACE_OS::fprintf (file, "INVALID_PRIORITY");
      break;

    // The following are only used during scheduling (in the case of
    // off-line scheduling, they are only used prior to runtime).
    // To save a little code space (280 bytes on g++ 2.7.2/Solaris 2.5.1),
    // we could conditionally compile them so that they're not in the
    // runtime version.
    case ST_UTILIZATION_BOUND_EXCEEDED :
      ACE_OS::fprintf (file, "UTILIZATION_BOUND_EXCEEDED");
      break;
    case ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS :
      ACE_OS::fprintf (file, "INSUFFICIENT_THREAD_PRIORITY_LEVELS");
      break;
    case ST_CYCLE_IN_DEPENDENCIES :
      ACE_OS::fprintf (file, "CYCLE_IN_DEPENDENCIES");
      break;
    case UNABLE_TO_OPEN_SCHEDULE_FILE :
      ACE_OS::fprintf (file, "UNABLE_TO_OPEN_SCHEDULE_FILE");
      break;
    case UNABLE_TO_WRITE_SCHEDULE_FILE :
      ACE_OS::fprintf (file, "UNABLE_TO_WRITE_SCHEDULE_FILE");
      break;
    // End of config-only status values.

    default:
      ACE_OS::fprintf (file, "UNKNOWN STATUS: %d", status);
  }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class ACE_Scheduler member functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ACE_Scheduler::ACE_Scheduler () :
  minimum_priority_queue_ (0),  // Could initialize this to -1, but it's
                                // unsigned and we don't really need to
                                // distinguish between no queues and one
                                // queue.
  modes_ (0),
  tasks_ (0),
  threads_ (0),
  mode_ (0),
  status_ (NOT_SCHEDULED),
  output_level_ (0)
{
}


ACE_Scheduler::~ACE_Scheduler ()
{
}


// ************************************************************

ACE_Scheduler::status_t
ACE_Scheduler::get_rt_info (Object_Name name,
                        RT_Info* &rtinfo)
{
  handle_t handle;

  // This makes a copy.  We can optimize this with our own string
  // class.
  ACE_CString lookup (name);
  // Search the map for the <name>.  If found, return the RT_Info.
  RT_Info **info_array = 0;
  if (info_collection_.find (lookup, info_array) >= 0)
    {
      rtinfo = info_array[0];
      // If we find it, return.
      return SUCCEEDED;
    }
  else
    // Otherwise, make one, bind it, and register it.
    {
      rtinfo = new RT_Info;
      rtinfo->entry_point = name;
      // Create and array (size one) of RT_Info*
      info_array = new RT_Info*[1];
      info_array[0] = rtinfo;
      // Bind the rtinfo to the name.
      if (info_collection_.bind (lookup, info_array) != 0)
        {
          delete rtinfo;
          delete info_array;
          rtinfo = 0;
          return FAILED; // Error!
        }
      else
        {
          // Register the array.
          status_t result = this->register_task (info_array, 1, handle);
          if (result == SUCCEEDED)
            {
              rtinfo->handle = handle;
              return ST_UNKNOWN_TASK; // Didn't find it, but made one!
            }
          else
            {
              rtinfo->handle = 0;
              return FAILED;
            }
        }
    }
}



int ACE_Scheduler::number_of_dependencies(RT_Info* rt_info)
{
  return rt_info->dependencies.length();
}

int ACE_Scheduler::number_of_dependencies(RT_Info& rt_info)
{
  return rt_info.dependencies.length();
}

int ACE_Scheduler::add_dependency(RT_Info* rt_info,
                              const Dependency_Info& d)
{
  ACE_DEBUG ((LM_DEBUG, "adding dependecy to: %s\n",
              (const char*)rt_info->entry_point));
  RtecScheduler::Dependency_Set& set = rt_info->dependencies;
  int l = set.length();
  set.length(l + 1);
  set[l] = d;
  return 0;
}

void ACE_Scheduler::export(RT_Info* info, FILE* file)
{
  export(*info, file);
}

void ACE_Scheduler::export(RT_Info& info, FILE* file)
{
  // The divide-by-1 is for ACE_U_LongLong support.
  (void) ACE_OS::fprintf (file,
                          "%s\n%d\n%ld\n%ld\n%ld\n%ld\n%d\n%ld\n%u\n"
                          "# begin dependencies\n%d\n",
                          (const char*)info.entry_point,
                          info.handle,
                          ORBSVCS_Time::to_hrtime (info.worst_case_execution_time) / 1,
                          ORBSVCS_Time::to_hrtime (info.typical_execution_time) / 1,
                          ORBSVCS_Time::to_hrtime (info.cached_execution_time) / 1,
                          info.period,
                          info.importance,
                          ORBSVCS_Time::to_hrtime (info.quantum) / 1,
                          info.threads,
                          number_of_dependencies(info));

  for (int i = 0; i < number_of_dependencies(info); ++i)
    {
      RT_Info tmp;
      // TODO: info.dependencies [i].rt_info >>= &tmp;
      (void) ACE_OS::fprintf (file, "%s, %d\n",
                              (const char*)tmp.entry_point,
                              info.dependencies[i].number_of_calls);

    }

  (void) ACE_OS::fprintf (file, "# end dependencies\n%d\n%d\n%d\n\n",
                          info.priority,
                          info.dynamic_subpriority,
                          info.static_subpriority);


}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Entry<ACE_CString, ACE_Scheduler::RT_Info **>;

template class ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_CString,
                               ACE_Scheduler::RT_Info **,
                               ACE_SYNCH_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_CString, ACE_Scheduler::RT_Info **,
                                     ACE_SYNCH_MUTEX>;
template class ACE_Map_Iterator<ACE_CString, ACE_Scheduler::RT_Info **,
                                ACE_SYNCH_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_CString, ACE_Scheduler::RT_Info **,
                                        ACE_SYNCH_MUTEX>;
template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Entry<ACE_CString, ACE_Scheduler::RT_Info **>

#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// EOF
