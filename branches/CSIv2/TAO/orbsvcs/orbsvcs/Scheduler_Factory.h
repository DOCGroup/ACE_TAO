/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Scheduler_Factory.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_SCHEDULER_FACTORY_H
#define ACE_SCHEDULER_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Sched/sched_export.h"

class TAO_RTSched_Export ACE_Scheduler_Factory
{
  // = TITLE
  //   Factory of scheduler services.
  //
  // = DESCRIPTION
  //   This class acts as a factory for scheduler servers. At config
  //   time it will return a remote server, which will actually
  //   compute the scheduling parameters. At run-time it returns a
  //   local server, which will use the results of the config runs to
  //   actually do the scheduling, without incurring in RPC overheads.
public:
  enum Factory_Status 
  {
    // = TITLE
    //   Factory Status
    //
    // = DESCRIPTION
    //   This type enumerates the possible states of the factory:
    //   uninitialized, or in a configuration, runtime, or 
    //   reconfigurable mode of operation.

    UNINITIALIZED,
    CONFIG,
    RECONFIG,
    RUNTIME
  };

  struct POD_RT_Info
  {
    // = TITLE
    //   Plain Old Data for RT_Infos.
    //
    // = DESCRIPTION
    //   This class provide us with a plain old data version of
    //   RT_Info, this is useful for implementing arrays of
    //   those.

    const char *entry_point;
    RtecScheduler::handle_t handle;
    RtecScheduler::Time worst_case_execution_time;
    RtecScheduler::Time typical_execution_time;
    RtecScheduler::Time cached_execution_time;
    RtecScheduler::Period_t period;
    CORBA::Long criticality;
    CORBA::Long importance;
    RtecScheduler::Quantum_t quantum;
    CORBA::Long threads;
    RtecScheduler::OS_Priority priority;
    RtecScheduler::Preemption_Subpriority_t static_subpriority;
    RtecScheduler::Preemption_Priority_t preemption_priority;
    CORBA::Long info_type;
  };


  struct POD_Dependency_Info
  {
    // = TITLE
    //   Plain Old Data for RT_Info Dependencies.
    //
    // = DESCRIPTION
    //   This class provide us with a plain old data version of
    //   dependencies between RT_Infos.  This is useful for implementing
    //   arrays of those.

    RtecScheduler::handle_t info_that_depends;
    RtecScheduler::handle_t info_depended_on;
    RtecScheduler::Dependency_Type_t dependency_type;
    CORBA::Long number_of_calls;
  };

  struct POD_Config_Info
  {
    // = TITLE
    //   Plain Old Data for dispatch queue configuration information.
    //
    // = DESCRIPTION
    //   This class provide us with a plain old data version of
    //   configuration info, which is useful for implementing static arrays
    //   NOTE: if used in an array, the run-time scheduler requires that the
    //   array index match the preemption priority stored in the config info
    //   at that index: this is used to detect uninitialized/corrupted schedules
    RtecScheduler::Preemption_Priority_t preemption_priority;
    RtecScheduler::OS_Priority thread_priority;
    RtecScheduler::Dispatching_Type_t dispatching_type;
  };

  struct POD_Scheduling_Anomaly
  {
    // = TITLE
    //   Plain Old Data for scheduling anomaly information.
    //
    // = DESCRIPTION
    //   This class provide us with a plain old data version of
    //   scheduling anomalies, which is used to generate error
    //   and warning lines in the runtime scheduling header output.

    const char* description;
    RtecScheduler::Anomaly_Severity severity;
  };

  static int use_config (CosNaming::NamingContext_ptr naming);
  // Setup the variables needed for a config run, using the
  // NamingContext to locate a Scheduler.

  static int use_config (CosNaming::NamingContext_ptr naming,
                         const char* name);
  // Setup the variables needed for a config run, using the
  // NamingContext to locate a Scheduler.

  static int use_runtime (int cc,
                          POD_Config_Info cfgi[],
                          int ec,
                          POD_RT_Info rti[]);
  // Disable config runs in the Factory and sets up the precomputed
  // scheduling information.

  static int server (RtecScheduler::Scheduler_ptr);
  static RtecScheduler::Scheduler_ptr server (void);
  // Return the Real-time Scheduling Service used for this run.
  // Must have been configured either using use_context() or use_data().
  //
  // Normally use_data() is called at static elaboration time, so
  // everything is automatic.  On config runs use_context() is called
  // from main, after resolve_initial_references.

  static int dump_schedule (const RtecScheduler::RT_Info_Set& infos,
                            const RtecScheduler::Config_Info_Set& configs,
                            const RtecScheduler::Scheduling_Anomaly_Set& anomalies,
                            const char* file_name = 0,
                            const char* rt_info_format = 0,
                            const char* config_info_format = 0);
  // This helper function will dump the schedule returned by a
  // RtecScheduler::Scheduler into a file, the file can be compiled to
  // create an efficient local implementation of the Scheduler.

  // TODO: How to do cleanup()? Use the ACE_Object_Manager stuff?

  static Factory_Status status (void);
  // This helper function allows the application to determine whether
  // the factory is uninitialized, or in a config or runtime mode of
  // operation.

  // = Access the (OS independent) preemption priority of the calling thread.
  static RtecScheduler::Preemption_Priority_t preemption_priority ();
  // Returns (u_int) -1 if the preemption priority hadn't been set.

  // = Set the (OS independent) preemption priority of the calling thread.
  static void set_preemption_priority
    (const RtecScheduler::Preemption_Priority_t);
  // The application or Event Channel is responsible for making sure
  // that the preemption priority is set before any access of the
  // preemption priority.

protected:
  static int no_config_run (void);
  // By default this factory assumes we are runnning a config
  // run. Calling this method disables that.  Since the methods
  // returns an int it can be used to initialize a static variable,
  // hence calling it before main(); this technique can be used in the
  // code emitted for the run-time scheduler, automagically disabling
  // the config_run() when that code is linked in.

private:
  static RtecScheduler::Scheduler_ptr server_;
  static Factory_Status status_;
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Scheduler_Factory.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_SCHEDULER_FACTORY_H */









