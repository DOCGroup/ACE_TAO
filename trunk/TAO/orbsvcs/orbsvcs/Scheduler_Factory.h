// $Id$

#ifndef ACE_SCHEDULER_FACTORY_H
#define ACE_SCHEDULER_FACTORY_H

#include "ace/OS.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/RtecSchedulerC.h"

class TAO_ORBSVCS_Export ACE_Scheduler_Factory
  // = TITLE
  //   Factory of scheduler services.
  //
  // = DESCRIPTION
  //   This class acts as a factory for scheduler servers. At config
  //   time it will return a remote server, which will actually
  //   compute the scheduling parameters. At run-time it returns a
  //   local server, which will use the results of the config runs to
  //   actually do the scheduling, without incurring in RPC overheads.
{
public:
  struct POD_RT_Info
    // = TITLE
    //   Plain Old Data for RT_Infos.
    //
    // = DESCRIPTION
    //   This class provide us with a plain old data version of
    //   RT_Info, this is useful for implementing static arrays or  of
    //   those.
  {
    const char* entry_point;
    RtecScheduler::handle_t handle;
    RtecScheduler::Time worst_case_execution_time;
    RtecScheduler::Time typical_execution_time;
    RtecScheduler::Time cached_execution_time;
    RtecScheduler::Period period;
    CORBA::Long criticality;
    CORBA::Long importance;
    RtecScheduler::Quantum quantum;
    CORBA::Long threads;
    RtecScheduler::OS_Priority priority;
    RtecScheduler::Sub_Priority static_subpriority;
    RtecScheduler::Sub_Priority dynamic_subpriority;
    RtecScheduler::Preemption_Priority preemption_priority;
    RtecScheduler::Info_Type info_type;
  };

  static int use_config (CosNaming::NamingContext_ptr naming);
  // Setup the variables needed for a config run, using the
  // NamingContext to locate a Scheduler.

  static int use_config (CosNaming::NamingContext_ptr naming,
			 const char* name);
  // Setup the variables needed for a config run, using the
  // NamingContext to locate a Scheduler.

  static int use_runtime (int entry_count,
                          POD_RT_Info rt_info[]);
  // Disable config runs in the Factory and setups the precomputed
  // scheduling.

  static RtecScheduler::Scheduler_ptr server (void);
  // Return the Real-time Scheduling Service used for this run.
  // Must have been configured either using use_context() or use_data().
  //
  // Normally use_data() is called at static elaboration time, so
  // everything is automatic.
  // On config runs use_context() is called from main, after
  // resolve_initial_references.

  static int dump_schedule (const RtecScheduler::RT_Info_Set& infos,
                            const char* file_name = 0);
  // This helper function will dump the schedule returned by a
  // RtecScheduler::Scheduler into a file, the file can be compiled to
  // create an efficient local implementation of the Scheduler.

  // TODO: How to do cleanup()? Use the ACE_Object_Manager stuff?

protected:
  static int no_config_run (void);
  // By default this factory assumes we are runnning a config
  // run. Calling this method disables that.
  // Since the methods returns an int it can be used to initialize a
  // static variable, hence calling it before main(); this technique
  // can be used in the code emitted for the run-time scheduler,
  // automagically disabling the config_run() when that code is linked
  // in.

private:
  static RtecScheduler::Scheduler_ptr server_;
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Scheduler_Factory.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SCHEDULER_FACTORY_H */
