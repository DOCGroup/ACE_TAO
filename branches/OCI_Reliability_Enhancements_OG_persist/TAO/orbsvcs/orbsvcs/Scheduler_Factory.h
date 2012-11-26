// -*- C++ -*-


//=============================================================================
/**
 *  @file    Scheduler_Factory.h
 *
 *  $Id$
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_SCHEDULER_FACTORY_H
#define ACE_SCHEDULER_FACTORY_H
#include /**/ "ace/pre.h"

#include "ace/Containers_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Sched/sched_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Reconfig_Scheduler_Entry;
struct TAO_RT_Info_Tuple;
typedef ACE_Ordered_MultiSet<TAO_RT_Info_Tuple *> TUPLE_SET;
typedef ACE_Ordered_MultiSet_Iterator<TAO_RT_Info_Tuple *> TUPLE_SET_ITERATOR;

/**
 * @class ACE_Scheduler_Factory
 *
 * @brief Factory of scheduler services.
 *
 * This class acts as a factory for scheduler servers. At config
 * time it will return a remote server, which will actually
 * compute the scheduling parameters. At run-time it returns a
 * local server, which will use the results of the config runs to
 * actually do the scheduling, without incurring in RPC overheads.
 */
class TAO_RTSched_Export ACE_Scheduler_Factory
{
public:
  /*
   * This type enumerates the possible states of the factory:
   * uninitialized, or in a configuration, runtime, or
   * reconfigurable mode of operation.
   */
  enum Factory_Status
  {
    UNINITIALIZED,
    CONFIG,
    RECONFIG,
    RUNTIME
  };

  /*
   * @brief Plain Old Data for RT_Infos.
   *
   * This class provide us with a plain old data version of
   * RT_Info, this is useful for implementing arrays of
   * those.
   */
  struct POD_RT_Info
  {

    const char *entry_point;
    RtecScheduler::handle_t handle;
    RtecScheduler::Time worst_case_execution_time;
    RtecScheduler::Time typical_execution_time;
    RtecScheduler::Time cached_execution_time;
    RtecScheduler::Period_t period;
    CORBA::Long criticality;
    CORBA::Long importance;
    RtecScheduler::Quantum_t quantum;
    RtecScheduler::Threads_t threads;
    RtecScheduler::OS_Priority priority;
    RtecScheduler::Preemption_Subpriority_t static_subpriority;
    RtecScheduler::Preemption_Priority_t preemption_priority;
    CORBA::Long info_type;
    RtecScheduler::RT_Info_Enabled_Type_t enabled;

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

    RtecScheduler::Dependency_Type_t dependency_type;
    CORBA::Long number_of_calls;
    RtecScheduler::handle_t info_that_depends;
    RtecScheduler::handle_t info_depended_on;
    RtecScheduler::Dependency_Enabled_Type_t enabled;
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

  /// Setup the variables needed for a config run, using the
  /// NamingContext to locate a Scheduler.
  static int use_config (CosNaming::NamingContext_ptr naming);

  /// Setup the variables needed for a config run, using the
  /// NamingContext to locate a Scheduler.
  static int use_config (CosNaming::NamingContext_ptr naming,
                         const char* name);

  /// Disable config runs in the Factory and sets up the precomputed
  /// scheduling information.
  static int use_runtime (int cc,
                          POD_Config_Info cfgi[],
                          int ec,
                          POD_RT_Info rti[]);

  /**
   * Return the Real-time Scheduling Service used for this run.
   * Must have been configured either using use_context() or use_data().
   *
   * Normally use_data() is called at static elaboration time, so
   * everything is automatic.  On config runs use_context() is called
   * from main, after resolve_initial_references.
   */
  static int server (RtecScheduler::Scheduler_ptr);
  static RtecScheduler::Scheduler_ptr server (void);

  /**
   * This helper function will dump the schedule returned by a
   * RtecScheduler::Scheduler into a file, the file can be compiled to
   * create an efficient local implementation of the Scheduler.
   */
  static int dump_schedule (const RtecScheduler::RT_Info_Set& infos,
                            const RtecScheduler::Dependency_Set& dependencies,
                            const RtecScheduler::Config_Info_Set& configs,
                            const RtecScheduler::Scheduling_Anomaly_Set& anomalies,
                            const ACE_TCHAR* file_name = 0,
                            const char* rt_info_format = 0,
                            const char* dependency_format = 0,
                            const char* config_info_format = 0,
                            int dump_disabled_infos = 0,
                            int dump_disabled_dependencies = 0);

  // TODO: How to do cleanup()? Use the ACE_Object_Manager stuff?

  /// This helper function prints out a single scheduling entry contents
  static void log_scheduling_entry(TAO_Reconfig_Scheduler_Entry * entry,
                                   FILE* file);

  /// This helper function prints out the intermediate scheduling entries
  static int log_scheduling_entries(TAO_Reconfig_Scheduler_Entry ** entry_ptr_array,
                                     long entry_ptr_array_size,
                                     const char* file_name);

  /// This helper function prints out the arry used to create scheduling entries
  /// sorted in topological order then priority order
  static void log_scheduling_tuples(TAO_RT_Info_Tuple ** tuple_ptr_array,
                                    long tuple_ptr_array_size,
                                    const char* file_name);

  static void log_tuple_subset(TUPLE_SET & tuple_subset,
                               FILE* file);

  /**
   * This helper function allows the application to determine whether
   * the factory is uninitialized, or in a config or runtime mode of
   * operation.
   */
  static Factory_Status status (void);

  // = Access the (OS independent) preemption priority of the calling thread.
  /// Returns (u_int) -1 if the preemption priority hadn't been set.
  static RtecScheduler::Preemption_Priority_t preemption_priority ();

  // = Set the (OS independent) preemption priority of the calling thread.
  /**
   * The application or Event Channel is responsible for making sure
   * that the preemption priority is set before any access of the
   * preemption priority.
   */
  static void set_preemption_priority
    (const RtecScheduler::Preemption_Priority_t);

  // Accessor for obtaining the default period (Boeing Extension)
  /// Method for setting the default period (Boeing Extension)
  static RtecScheduler::Period_t period_default();
  static void period_default(RtecScheduler::Period_t period_default);

  // Accessor for obtaining the default threads (Boeing Extension)
  /// Method for setting the default threads (Boeing Extension)
  static RtecScheduler::Threads_t threads_default();
  static void threads_default(RtecScheduler::Threads_t threads_default);

  // Accessor for obtaining the default importance (VERY_LOW_IMPORTANCE to VERY_HIGH_IMPORTANCE).  (Boeing Extension)
  /// Method for setting the default importance (VERY_LOW_IMPORTANCE to VERY_HIGH_IMPORTANCE).  (Boeing Extension)
  static RtecScheduler::Importance_t importance_default();
  static void importance_default(RtecScheduler::Importance_t importance_default);

  // Accessor for obtaining the default criticality (VERY_LOW_CRITICALITY to VERY_HIGH_CRITICALITY).  (Boeing Extension)
  /// Method for setting the default criticality (VERY_LOW_CRITICALITY to VERY_HIGH_CRITICALITY).  (Boeing Extension)
  static RtecScheduler::Criticality_t criticality_default();
  static void criticality_default(RtecScheduler::Criticality_t criticality_default);

  // Accessor for obtaining the default rt_info enabled state.  (RT_INFO_DISABLED, RT_INFO_ENABLED, or RT_INFO_NON_VOLATILE)
  static RtecScheduler::RT_Info_Enabled_Type_t rt_info_enable_state_default();

  // Method for setting the default rt_info enabled state.  (RT_INFO_DISABLED, RT_INFO_ENABLED, or RT_INFO_NON_VOLATILE)
  static void rt_info_enable_state_default(RtecScheduler::RT_Info_Enabled_Type_t rt_info_enable_state_default);

protected:

  /**
   * By default this factory assumes we are runnning a config
   * run. Calling this method disables that.  Since the methods
   * returns an int it can be used to initialize a static variable,
   * hence calling it before main(); this technique can be used in the
   * code emitted for the run-time scheduler, automagically disabling
   * the config_run() when that code is linked in.
   */
  static int no_config_run (void);

private:
  static RtecScheduler::Scheduler_ptr server_;

  static Factory_Status status_;
  /// Default period configuration.  (Boeing Extension)
  static RtecScheduler::Period_t period_default_;
  /// Default threads configuration.  (Boeing Extension)
  static RtecScheduler::Threads_t threads_default_;

  /// Default importance configuration.  (Boeing Extension)
  static RtecScheduler::Importance_t importance_default_;
  /// Default criticality.  (Boeing Extension)
  static RtecScheduler::Criticality_t criticality_default_;

  /// Default rt_info enabled state.  (Boeing Extension)
  static RtecScheduler::RT_Info_Enabled_Type_t rt_info_enable_state_default_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Scheduler_Factory.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_SCHEDULER_FACTORY_H */
