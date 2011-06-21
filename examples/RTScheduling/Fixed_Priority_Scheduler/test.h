//$Id$
#ifndef TEST_H
#define TEST_H
#include /**/ "ace/pre.h"


#include "FP_Scheduler.h"
#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "FP_DT_Creator.h"

class Task_Stats;

class DT_Test :public ACE_Task <ACE_SYNCH>
{
 public:

  DT_Test (void);

  void check_supported_priorities (void);

  int init (int argc, ACE_TCHAR *argv []);

  void run (int argc, ACE_TCHAR *argv []);

  void dt_creator (FP_DT_Creator* dt_creator);
  FP_DT_Creator* dt_creator (void);

  Fixed_Priority_Scheduler* scheduler (void);

  int activate_task (void);

  CORBA::ORB_ptr orb (void);

  ACE_hrtime_t base_t;

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
  FP_DT_Creator* dt_creator_;
  PortableServer::Servant_var<Fixed_Priority_Scheduler> scheduler_;
  RTCORBA::RTORB_var rt_orb_;
  /// Reference to the root poa.
  PortableServer::POA_var root_poa_;

};

typedef ACE_Singleton <DT_Test, TAO_SYNCH_MUTEX> DT_TEST;

#include /**/ "ace/post.h"
#endif /*TEST_H*/
