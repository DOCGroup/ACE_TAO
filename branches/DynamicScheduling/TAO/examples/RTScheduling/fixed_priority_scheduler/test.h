//$Id$
#ifndef TEST_H
#define TEST_H

#include "Thread_Task.h"
#include "FP_Scheduler.h"
#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "test_export.h"


typedef Thread_Task **DT_LIST;

class test_export DT_Test : public ACE_Service_Object
{
 public:
  void check_supported_priorities (void);
  
  void init_test (int argc, char *argv []
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void init_distributable_threads (int argc, char *argv []
				   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void create_distributable_threads (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

 private:
  /// = Policies
  long thr_sched_policy_;
  long thr_scope_policy_;
  ACE_Sched_Params::Policy sched_policy_;
  CORBA::ORB_var orb;
  int max_priority_;
  int min_priority_;
  RTScheduling::Current_var current_; 
  DT_LIST* dt_list_;
  Fixed_Priority_Scheduler* scheduler_;
  ACE_Barrier barrier_;
  long flags_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (test, DT_Test)

ACE_FACTORY_DECLARE (test, DT_Test)

#endif /*TEST_H*/
