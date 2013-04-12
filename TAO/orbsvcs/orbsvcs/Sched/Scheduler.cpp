//=============================================================================
/**
 *  @file    Scheduler.cpp
 *
 *  $Id$
 *
 *  @author David Levine
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
#include "ace/Sched_Params.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Sched/Scheduler.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Lock_Adapter_T.h"



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
          delete [] info_array;
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
  // ORBSVCS_DEBUG ((LM_DEBUG, "Sched (%t) adding dependency to: %s\n",
  //              (const char*)rt_info->entry_point));
  RtecScheduler::Dependency_Set& set = rt_info->dependencies;
  int l = set.length();
  set.length(l + 1);
  set[l] = d;
  return 0;
}

void ACE_Scheduler::export_to_file (RT_Info* info, FILE* file)
{
  ACE_Scheduler::export_to_file (*info, file);
}

void ACE_Scheduler::export_to_file (RT_Info& info, FILE* file)
{
  (void) ACE_OS::fprintf (file,
                          ACE_TEXT("%s\n%d\n")
                          ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT("\n")
                          ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT("\n")
                          ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT("\n")
                          ACE_TEXT("%d\n%d\n")
                          ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT("\n")
                          ACE_TEXT("%u\n# begin dependencies\n%d\n"),
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

  (void) ACE_OS::fprintf (file, "# end dependencies\n%d\n%d\n\n",
                          info.priority,
                          info.preemption_subpriority);


}

int
ACE_Scheduler::dispatch_configuration (const Preemption_Priority & p_priority,
                                       OS_Thread_Priority & priority,
                                       Dispatching_Type & d_type)
{
  // look up the stored configuration info for the given priority level
  Config_Info *config_info;
  if (lookup_config_info (p_priority, config_info) != SUCCEEDED)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "Config info for priority %lu could not be found\n",
                       p_priority),
                      -1);
  }

  priority = config_info->thread_priority;
  d_type = config_info->dispatching_type;

  return 0;
}
  // provide the thread priority and queue type for the given priority level
