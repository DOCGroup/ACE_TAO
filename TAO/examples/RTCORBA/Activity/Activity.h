/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Activity.h
 *
 *  $Id$
 *
 *  An activity is a process that contains Tasks.
 *  Each Task is composed of certain units of Jobs.
 *  A Job can perform work that is local/remote.
 *  Each Job is identified with a name.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "ace/Singleton.h"
#include "ace/Sched_Params.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/RTPortableServer/RTPortableServer.h"

class ACE_Barrier;
class Job_i;
class Periodic_Task;
class Builder;

/**
 * @class Activity
 *
 * @brief Driver class that maintains the orb and collections of objects for
 * generating activity in this process.
 *
 */
class Activity
{
  friend class ACE_Singleton<Activity, ACE_Null_Mutex>;

private:
  Activity ();
  ~Activity ();

public:
  /// initialize the ORB et. al.
  int init (int& argc, char *argv [] TAO_ENV_ARG_DECL);


  /// Activate the tasks, jobs, poas.
  void run (int argc, char *argv[] TAO_ENV_ARG_DECL);

  /// Resolve the naming service.
  int resolve_naming_service (TAO_ENV_SINGLE_ARG_DECL);

  /// = Accessors
  CORBA::ORB_ptr orb (void);
  RTCORBA::Current_ptr current (void);
  long scope_policy (void);
  long sched_policy (void);
  void builder (Builder* builder);

  /// Returns priority if server declared model else -1
  CORBA::Short get_server_priority (CORBA::Object_ptr server
                                    TAO_ENV_ARG_DECL);
  /// = Callbacks
  /// Task ended notification
  void task_ended (Periodic_Task* ended_task);

  /// Job shutdown notification
  void job_ended (Job_i* ended_job);

  protected:
  /// Init sched mode.
  int init_sched (void);

  /// = Activation methods.
  /// Activate the POA's
  void activate_poa_list (TAO_ENV_SINGLE_ARG_DECL);

  /// Activate the task list.
  void activate_schedule (TAO_ENV_SINGLE_ARG_DECL);

  /// Activate the Job's
  void activate_job_list (TAO_ENV_SINGLE_ARG_DECL);

  /// Check if we should process exit
  void check_ifexit (void);

  /// = Data members

  /// The Builder object.
  Builder* builder_;

  /// = Policies
  long thr_sched_policy_;
  long thr_scope_policy_;
  ACE_Sched_Params::Policy sched_policy_;

  /// ACE_Barrier to synch. tasks.
  ACE_Barrier* barrier_;

  /// Mutex to serialize access to our internal state.
  ACE_Lock* state_lock_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// RT ORB
  RTCORBA::RTORB_var rt_orb_;

  /// Current
  RTCORBA::Current_var current_;

  /// Reference to the root poa.
  PortableServer::POA_var root_poa_;

  /// A naming context.
  CosNaming::NamingContextExt_var naming_;

  /// Count the number of periodic tasks active.
  int active_task_count_;

  /// Count the number of Jobs active
  int active_job_count_;
};

typedef ACE_Singleton<Activity, ACE_Null_Mutex> ACTIVITY;

#endif /* ACTIVITY_H */
