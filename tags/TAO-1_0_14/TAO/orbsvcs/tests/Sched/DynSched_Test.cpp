// $Id$

#include "Strategy_Scheduler.h"
#if defined (ACE_HAS_QUANTIFY)
# include <quantify.h>
#endif /* ACE_HAS_QUANTIFY */

ACE_RCSID(Sched, DynSched_Test, "$Id$")

// period times, in 100 nanoseconds
#define ONE_HZ    10000000
#define FIVE_HZ    2000000
#define TEN_HZ     1000000
#define TWENTY_HZ   500000

typedef RtecScheduler::handle_t handle_t;
typedef RtecScheduler::Dependency_Info Dependency_Info;
typedef RtecScheduler::Preemption_Priority Preemption_Priority;
typedef RtecScheduler::OS_Priority OS_Priority;
typedef RtecScheduler::Sub_Priority Sub_Priority;
typedef RtecScheduler::RT_Info RT_Info;
typedef RtecScheduler::Time Time;
typedef RtecScheduler::Period Period;
typedef RtecScheduler::Info_Type Info_Type;
typedef RtecScheduler::Dependency_Type Dependency_Type;


static
void
setup_rt_info (RT_Info &info, const char *entry_point, u_long period,
               u_long execution, RtecScheduler::Criticality criticality,
               RtecScheduler::Importance importance, u_long threads)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  const TimeBase::ulonglong execution_time = {execution, 0};
  const TimeBase::ulonglong zero = {0, 0};

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = execution_time;
  info.typical_execution_time = execution_time;
  info.cached_execution_time = zero;
  info.period = period;
  info.criticality = criticality;
  info.importance = importance;
  info.quantum = zero;
  info.threads = threads;
  info.priority = 0;
  info.dynamic_subpriority = 0;
  info.static_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::OPERATION;
  info.volatile_token = 0;
}

static
int
register_rt_info (ACE_DynScheduler &scheduler,
                  RtecScheduler::RT_Info &info)
{
  int result = 0;

  if (scheduler.register_task (&info, info.handle) !=
      ACE_DynScheduler::SUCCEEDED)
  {
     result = 1;
     printf ("Could not register info for \"%s\"\n", info.entry_point.in ());
  }

  return result;
}

#if 0 /* not currently used */
static
void
setup_conjunction (RT_Info &info, const char *entry_point)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  const TimeBase::ulonglong zero = {0, 0};

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = zero;
  info.typical_execution_time = zero;
  info.cached_execution_time = zero;
  info.period = 0;
  info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  info.quantum = zero;
  info.threads = 0;
  info.priority = 0;
  info.dynamic_subpriority = 0;
  info.static_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::CONJUNCTION;
  info.volatile_token = 0;
}

static
void
setup_disjunction (RT_Info &info, const char *entry_point)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  const TimeBase::ulonglong zero = {0, 0};

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = zero;
  info.typical_execution_time = zero;
  info.cached_execution_time = zero;
  info.period = 0;
  info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  info.quantum = zero;
  info.threads = 0;
  info.priority = 0;
  info.dynamic_subpriority = 0;
  info.static_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::DISJUNCTION;
  info.volatile_token = 0;
}
#endif /* not currently used */


static
int
run_schedule (ACE_Scheduler_Strategy &strategy,
              const char *output_filename, const char *heading)
{
//  RtecScheduler::RT_Info low_10, low_20, high_10, high_20;
  RtecScheduler::RT_Info low_1, low_5, low_10, low_20;
  RtecScheduler::RT_Info high_1, high_5, high_10, high_20;

  ACE_Strategy_Scheduler scheduler (strategy);

  setup_rt_info (low_1,   "low_1",   ONE_HZ,    180000,
                 RtecScheduler::LOW_CRITICALITY,
                 RtecScheduler::HIGH_IMPORTANCE, 1);
  setup_rt_info (low_5,   "low_5",   FIVE_HZ,   180000,
                 RtecScheduler::LOW_CRITICALITY,
                 RtecScheduler::HIGH_IMPORTANCE, 1);
  setup_rt_info (low_10,  "low_10",  TEN_HZ,    180000,
                 RtecScheduler::LOW_CRITICALITY,
                 RtecScheduler::HIGH_IMPORTANCE, 1);
  setup_rt_info (low_20,  "low_20",  TWENTY_HZ, 180000,
                 RtecScheduler::LOW_CRITICALITY,
                 RtecScheduler::HIGH_IMPORTANCE, 1);
  setup_rt_info (high_1,  "high_1",  ONE_HZ,    180000,
                 RtecScheduler::HIGH_CRITICALITY,
                 RtecScheduler::LOW_IMPORTANCE,  1);
  setup_rt_info (high_5,  "high_5",  FIVE_HZ,   180000,
                 RtecScheduler::HIGH_CRITICALITY,
                 RtecScheduler::LOW_IMPORTANCE,  1);
  setup_rt_info (high_10, "high_10", TEN_HZ,    180000,
                 RtecScheduler::HIGH_CRITICALITY,
                 RtecScheduler::LOW_IMPORTANCE,  1);
  setup_rt_info (high_20, "high_20", TWENTY_HZ, 180000,
                 RtecScheduler::HIGH_CRITICALITY,
                 RtecScheduler::LOW_IMPORTANCE,  1);

  if (
      register_rt_info (scheduler, low_1)   ||
      register_rt_info (scheduler, low_5)   ||
      register_rt_info (scheduler, low_10)  ||
      register_rt_info (scheduler, low_20)  ||
      register_rt_info (scheduler, high_1)  ||
      register_rt_info (scheduler, high_5)  ||
      register_rt_info (scheduler, high_10) ||
      register_rt_info (scheduler, high_20))
  {
    return -1;
  }

#if defined (ACE_HAS_QUANTIFY)
  quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */
  ACE_DynScheduler::status_t status = scheduler.schedule ();
#if defined (ACE_HAS_QUANTIFY)
  quantify_stop_recording_data ();
#endif /* ACE_HAS_QUANTIFY */

  switch (status)
  {
    case ACE_DynScheduler::SUCCEEDED:
    case ACE_DynScheduler::ST_UTILIZATION_BOUND_EXCEEDED:

      status = scheduler.output_timeline (output_filename, heading);
      switch (status)
      {
        case ACE_DynScheduler::SUCCEEDED:
        case ACE_DynScheduler::ST_UTILIZATION_BOUND_EXCEEDED:
          break;

        default :
          printf ("scheduler.output_timeline (\"%s\") failed: returned %d\n",
                  output_filename, status);
         return -1;
      }

      break;

    default :

      printf ("scheduler.schedule () failed: returned %d\n", status);
      return -1;
  }

  return 0;
}

int
main (int, char *[])
{
  int result = 0;

  // create a bunch of different strategies, indicating the minimum critical
  // priority level (number of priority levels in critical set - 1) for each.

  ACE_RMS_Scheduler_Strategy rms_strategy (3);
//  ACE_RMS_Scheduler_Strategy rms_strategy (1);

  ACE_MLF_Scheduler_Strategy mlf_strategy (0);
  ACE_EDF_Scheduler_Strategy edf_strategy (0);

  ACE_RMS_Dyn_Scheduler_Strategy rms_dyn_strategy (3);
//  ACE_RMS_Dyn_Scheduler_Strategy rms_dyn_strategy (1);

  ACE_MUF_Scheduler_Strategy muf_strategy (0);

  result = run_schedule (rms_strategy, "RMS_Timelines",
                         "RMS Scheduling Strategy");
  if (result < 0)
  {
    printf ("run_schedule (rms_strategy, \"RMS_Timelines\", "
            "\"RMS Scheduling Strategy\") returned %d\n",
            result);
    return 1;
  }

  result = run_schedule (mlf_strategy, "MLF_Timelines",
                         "MLF Scheduling Strategy");
  if (result < 0)
  {
    printf ("run_schedule (mlf_strategy, \"MLF_Timelines\", "
            "\"MLF Scheduling Strategy\") returned %d\n",
            result);
    return 1;
  }

  result = run_schedule (edf_strategy, "EDF_Timelines",
                         "EDF Scheduling Strategy");
  if (result < 0)
  {
    printf ("run_schedule (edf_strategy, \"EDF_Timelines\", "
            "\"EDF Scheduling Strategy\") returned %d\n",
            result);
    return 1;
  }

  result = run_schedule (rms_dyn_strategy, "RMS_Dyn_Timelines",
                         "RMS-Dynamic Scheduling Strategy");
  if (result < 0)
  {
    printf ("run_schedule (rms_dyn_strategy, \"RMS_Dyn_Timelines\", "
            "\"RMS-Dynamic Scheduling Strategy\") returned %d\n",
            result);
    return 1;
  }

  result = run_schedule (muf_strategy, "MUF_Timelines",
                         "MUF Scheduling Strategy");
  if (result < 0)
  {
    printf ("run_schedule (muf_strategy, \"MUF_Timelines\", "
            "\"MUF Scheduling Strategy\") returned %d\n",
            result);
    return 1;
  }

  return 0;
}
