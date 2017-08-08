/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Activity.h
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
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Null_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

class Job_i;
class Periodic_Task;
class Builder;

/**
 * @class Activity
 *
 * @brief Driver class that maintains the orb and collections of objects for
 * generating activity in this process.
 */
class Activity
{
  friend class ACE_Singleton<Activity, ACE_Null_Mutex>;

private:
  Activity ();
  ~Activity ();

public:
  /// initialize the ORB et. al.
  int init (int& argc, ACE_TCHAR *argv []);


  /// Activate the tasks, jobs, poas.
  void run (int argc, ACE_TCHAR *argv[]);

  /// Resolve the naming service.
  int resolve_naming_service (void);

  /// = Accessors
  CORBA::ORB_ptr orb (void);
  RTCORBA::Current_ptr current (void);
  void builder (Builder* builder);

  /// Returns priority if server declared model else -1
  CORBA::Short get_server_priority (CORBA::Object_ptr server);
  /// = Callbacks
  /// Task ended notification
  void task_ended (Periodic_Task* ended_task);

  /// Job shutdown notification
  void job_ended (Job_i* ended_job);

protected:
  /// = Activation methods.
  /// Activate the POA's
  void activate_poa_list (void);

  /// Activate the task list.
  void activate_schedule (void);

  /// Activate the Job's
  void activate_job_list (void);

  /// Check if we should process exit
  void check_ifexit (void);

  /// Create a file whose name is specified in the -Started_Flag <file_name> argument at startup.
  /// This file flags that the Activity has finished its bootstrapping step.
  void create_started_flag_file (int argc, ACE_TCHAR *argv[]);

  /// = Data members

  /// The Builder object.
  Builder* builder_;

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
  CosNaming::NamingContextExt_ptr naming_;

  /// The Priority Mapping helper.
  RTCORBA::PriorityMapping *priority_mapping_;

  /// Count the number of periodic tasks active.
  int active_task_count_;

  /// Count the number of Jobs active
  int active_job_count_;
};

typedef ACE_Singleton<Activity, ACE_Null_Mutex> ACTIVITY;

#endif /* ACTIVITY_H */
