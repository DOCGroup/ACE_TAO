//$Id$
#ifndef TEST_H
#define TEST_H
#include "ace/pre.h"


#include "MIF_Scheduler.h"
#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "MIF_DT_Creator.h"

class Task_Stats;

class DT_Test :public ACE_Task <ACE_SYNCH>
{
 public:
  
  DT_Test (void);
  
  int init (int argc, char *argv []
	    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void run (int argc, char *argv [] 
	  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void dt_creator (MIF_DT_Creator* dt_creator);

  MIF_Scheduler* scheduler (void);

  int activate_task (void);
  protected:
  /// task svc
  virtual int svc (void);

 private:
  /// = Policies
  long thr_sched_policy_;
  long thr_scope_policy_;
  ACE_Sched_Params::Policy sched_policy_;
  CORBA::ORB_var orb_;
  int max_priority_;
  int min_priority_;
  RTScheduling::Current_var current_; 
  MIF_DT_Creator* dt_creator_;
  MIF_Scheduler* scheduler_;
};

typedef ACE_Singleton <DT_Test, ACE_Thread_Mutex> DT_TEST;

#include "ace/post.h"
#endif /*TEST_H*/
