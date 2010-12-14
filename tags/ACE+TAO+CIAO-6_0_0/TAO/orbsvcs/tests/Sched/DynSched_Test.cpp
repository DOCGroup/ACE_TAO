// $Id$

#include "orbsvcs/Sched/Strategy_Scheduler.h"
#include "orbsvcs/Time_Utilities.h"
#if defined (ACE_HAS_QUANTIFY)
# include <quantify.h>
#endif /* ACE_HAS_QUANTIFY */

// period times, in 100 nanoseconds
#define ONE_HZ    10000000
#define FIVE_HZ    2000000
#define TEN_HZ     1000000
#define TWENTY_HZ   500000

class DynSched_Test
  // = TITLE
  //    DynSched Test wrapper
  // = DESCRIPTION
  //    Wraps static functions for test
{
public:

  static void setup_rt_info (RtecScheduler::RT_Info &info,
                             const char *entry_point,
                             u_long period,
                             RtecScheduler::Time execution,
                             RtecScheduler::Criticality_t criticality,
                             RtecScheduler::Importance_t importance,
                             u_long threads);
  // Initializes an RT_Info.

  static int register_rt_info (ACE_DynScheduler &scheduler,
                               RtecScheduler::RT_Info &info);
  // Registers an RT_Info.


#if 0 /* not currently used */
  static void setup_conjunction (RtecScheduler::RT_Info &info,
                                 const char *entry_point);
  // Sets up a conjunction.

  static void setup_disjunction (RtecScheduler::RT_Info &info,
                                 const char *entry_point);
  // Sets up a disjunction.

#endif /* not currently used */

  static int run_schedule (ACE_Scheduler_Strategy &strategy,
                           const char *output_filename,
                           const char *heading);
  // Creates the schedule, runs timelines.

};


// Initializes an RT_Info.

void
DynSched_Test::setup_rt_info (RtecScheduler::RT_Info &info,
                              const char *entry_point,
                              u_long period,
                              RtecScheduler::Time execution,
                              RtecScheduler::Criticality_t criticality,
                              RtecScheduler::Importance_t importance,
                              u_long threads)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = execution;
  info.typical_execution_time = execution;
  info.cached_execution_time = ORBSVCS_Time::zero ();
  info.period = period;
  info.criticality = criticality;
  info.importance = importance;
  info.quantum = ORBSVCS_Time::zero ();
  info.threads = threads;
  info.priority = 0;
  info.preemption_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::OPERATION;
  info.volatile_token = 0;
}


// Registers an RT_Info.

int
DynSched_Test::register_rt_info (ACE_DynScheduler &scheduler,
                                 RtecScheduler::RT_Info &info)
{
  int result = 0;

  if (scheduler.register_task (&info, info.handle) !=
      ACE_DynScheduler::SUCCEEDED)
  {
     result = 1;
     ACE_OS::printf ("Could not register info for \"%s\"\n", info.entry_point.in ());
  }

  return result;
}

#if 0 /* not currently used */

// Sets up a conjunction.

void
DynSched_Test::setup_conjunction (RtecScheduler::RT_Info &info,
                                  const char *entry_point)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = ORBSVCS_Time::zero ();
  info.typical_execution_time = ORBSVCS_Time::zero ();
  info.cached_execution_time = ORBSVCS_Time::zero ();
  info.period = 0;
  info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  info.quantum = ORBSVCS_Time::zero ();
  info.threads = 0;
  info.priority = 0;
  info.preemption_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::CONJUNCTION;
  info.volatile_token = 0;
}


// Sets up a disjunction.

void
DynSched_Test::setup_disjunction (RtecScheduler::RT_Info &info,
                                  const char *entry_point)
{
  // copy the passed entry point string into the RT_Info
  info.entry_point = CORBA::string_dup (entry_point);

  // initialize other values
  info.handle = 0;
  info.worst_case_execution_time = ORBSVCS_Time::zero ();
  info.typical_execution_time = ORBSVCS_Time::zero ();
  info.cached_execution_time = ORBSVCS_Time::zero ();
  info.period = 0;
  info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  info.quantum = ORBSVCS_Time::zero ();
  info.threads = 0;
  info.priority = 0;
  info.preemption_subpriority = 0;
  info.preemption_priority = 0;
  info.info_type = RtecScheduler::DISJUNCTION;
  info.volatile_token = 0;
}
#endif /* not currently used */


// Creates the schedule, runs timelines.

int
DynSched_Test::run_schedule (ACE_Scheduler_Strategy &strategy,
                             const char *output_filename,
                             const char *heading)
{
  RtecScheduler::RT_Info low_1, low_5, low_10, low_20;
  RtecScheduler::RT_Info high_1, high_5, high_10, high_20;

  ACE_Strategy_Scheduler scheduler (strategy);

  DynSched_Test::setup_rt_info (low_1,   "low_1", ONE_HZ,    180000,
                                RtecScheduler::LOW_CRITICALITY,
                                RtecScheduler::HIGH_IMPORTANCE, 1);
  DynSched_Test::setup_rt_info (low_5,   "low_5",   FIVE_HZ,   180000,
                                RtecScheduler::LOW_CRITICALITY,
                                RtecScheduler::HIGH_IMPORTANCE, 1);
  DynSched_Test::setup_rt_info (low_10,  "low_10",  TEN_HZ,    180000,
                                RtecScheduler::LOW_CRITICALITY,
                                RtecScheduler::HIGH_IMPORTANCE, 1);
  DynSched_Test::setup_rt_info (low_20,  "low_20",  TWENTY_HZ, 180000,
                                RtecScheduler::LOW_CRITICALITY,
                                RtecScheduler::HIGH_IMPORTANCE, 1);
  DynSched_Test::setup_rt_info (high_1,  "high_1",  ONE_HZ,    180000,
                                RtecScheduler::HIGH_CRITICALITY,
                                RtecScheduler::LOW_IMPORTANCE,  1);
  DynSched_Test::setup_rt_info (high_5,  "high_5",  FIVE_HZ,   180000,
                                RtecScheduler::HIGH_CRITICALITY,
                                RtecScheduler::LOW_IMPORTANCE,  1);
  DynSched_Test::setup_rt_info (high_10, "high_10", TEN_HZ,    180000,
                                RtecScheduler::HIGH_CRITICALITY,
                                RtecScheduler::LOW_IMPORTANCE,  1);
  DynSched_Test::setup_rt_info (high_20, "high_20", TWENTY_HZ, 180000,
                                RtecScheduler::HIGH_CRITICALITY,
                                RtecScheduler::LOW_IMPORTANCE,  1);

  if (
      DynSched_Test::register_rt_info (scheduler, low_1)   ||
      DynSched_Test::register_rt_info (scheduler, low_5)   ||
      DynSched_Test::register_rt_info (scheduler, low_10)  ||
      DynSched_Test::register_rt_info (scheduler, low_20)  ||
      DynSched_Test::register_rt_info (scheduler, high_1)  ||
      DynSched_Test::register_rt_info (scheduler, high_5)  ||
      DynSched_Test::register_rt_info (scheduler, high_10) ||
      DynSched_Test::register_rt_info (scheduler, high_20))
  {
    return -1;
  }

#if defined (ACE_HAS_QUANTIFY)
  quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */

  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> anomaly_set;
  ACE_DynScheduler::status_t status = scheduler.schedule (anomaly_set);

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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "scheduler.output_timeline (\"%s\") failed: "
                             "returned %d\n",
                             output_filename, status), -1);
      }

      break;

    default :

      ACE_ERROR_RETURN ((LM_ERROR,
                         "scheduler.schedule () failed: "
                         "returned %d\n",
                         status), -1);
  }

  return 0;
}

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  int result = 0;

  // create a bunch of different strategies, indicating the minimum critical
  // priority level (number of priority levels in critical set - 1) for each.

  ACE_RMS_Scheduler_Strategy rms_strategy (3);

  ACE_MLF_Scheduler_Strategy mlf_strategy (0);
  ACE_EDF_Scheduler_Strategy edf_strategy (0);

  ACE_MUF_Scheduler_Strategy muf_strategy (0);

  result =
    DynSched_Test::run_schedule (rms_strategy, "RMS_Timelines",
                                 "RMS Scheduling Strategy");
  if (result < 0)
  {
    ACE_OS::printf ("run_schedule (rms_strategy, \"RMS_Timelines\", "
                    "\"RMS Scheduling Strategy\") returned %d\n",
                    result);
    return 1;
  }

  result =
    DynSched_Test::run_schedule (mlf_strategy, "MLF_Timelines",
                                 "MLF Scheduling Strategy");
  if (result < 0)
  {
    ACE_OS::printf ("run_schedule (mlf_strategy, \"MLF_Timelines\", "
                    "\"MLF Scheduling Strategy\") returned %d\n",
                    result);
    return 1;
  }

  result =
    DynSched_Test::run_schedule (edf_strategy, "EDF_Timelines",
                                 "EDF Scheduling Strategy");
  if (result < 0)
  {
    ACE_OS::printf ("run_schedule (edf_strategy, \"EDF_Timelines\", "
                    "\"EDF Scheduling Strategy\") returned %d\n",
                    result);
    return 1;
  }

  result =
    DynSched_Test::run_schedule (muf_strategy, "MUF_Timelines",
                                 "MUF Scheduling Strategy");
  if (result < 0)
  {
    ACE_OS::printf ("run_schedule (muf_strategy, \"MUF_Timelines\", "
                    "\"MUF Scheduling Strategy\") returned %d\n",
                    result);
    return 1;
  }

  return 0;
}
