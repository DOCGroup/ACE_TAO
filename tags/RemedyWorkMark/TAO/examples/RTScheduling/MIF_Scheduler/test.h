//$Id$
#ifndef TEST_H
#define TEST_H

#include "MIF_Scheduler.h"
#include "MIF_DT_Creator.h"

#include "ace/Sched_Params.h"
#include "tao/ORB_Core.h"
#include "ace/Synch.h"
#include "ace/Task.h"

class Task_Stats;

class DT_Test //:public ACE_Task <ACE_SYNCH>
{
 public:

  DT_Test (void);

  int init (int argc, ACE_TCHAR *argv []);

  void run (int argc, ACE_TCHAR *argv []);

  void dt_creator (MIF_DT_Creator* dt_creator);

  MIF_Scheduler* scheduler (void);

  int activate_task (void);


/*   protected: */
/*   /// task svc */
/*   virtual int svc (void); */


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
  PortableServer::Servant_var<MIF_Scheduler> scheduler_;
};

typedef ACE_Singleton <DT_Test, TAO_SYNCH_MUTEX> DT_TEST;

#endif /*TEST_H*/
