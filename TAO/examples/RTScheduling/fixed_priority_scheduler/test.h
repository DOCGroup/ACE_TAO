//$Id$
#ifndef TEST_H
#define TEST_H
#include "ace/pre.h"

#include "Thread_Task.h"
#include "FP_Scheduler.h"
#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "ace/Synch.h"
#include "DT_Creator.h"

class DT_Test 
{
 public:
  
  DT_Test (void);
  
  void check_supported_priorities (void);
  
  int init (int argc, char *argv []
	    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void run (int argc, char *argv [] 
	  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void dt_creator (DT_Creator* dt_creator);

  Fixed_Priority_Scheduler* scheduler (void);
  
  RTScheduling::Current_ptr current (void); 

 private:
  /// = Policies
  long thr_sched_policy_;
  long thr_scope_policy_;
  ACE_Sched_Params::Policy sched_policy_;
  CORBA::ORB_var orb_;
  int max_priority_;
  int min_priority_;
  RTScheduling::Current_var current_; 
  DT_Creator* dt_creator_;
  Fixed_Priority_Scheduler* scheduler_;
};

typedef ACE_Singleton <DT_Test, ACE_Null_Mutex> DT_TEST;

#include "ace/post.h"
#endif /*TEST_H*/
