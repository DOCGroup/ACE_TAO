//$Id$
#ifndef DT_CREATOR_H
#define DT_CREATOR_H

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "tao/RTScheduling/RTScheduler.h"


class Thread_Task;
class Task;

typedef Thread_Task **DT_LIST;

class DT_Creator : public ACE_Service_Object
{
 public:

  ~DT_Creator (void);

  int init (int argc, char *argv []);
  
  void create_distributable_threads (CORBA::ORB_ptr orb,
				     RTScheduling::Current_ptr  current
				     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  
  virtual void yield (int suspend_time) = 0;

  virtual CORBA::Policy_ptr sched_param (int importance) = 0;

  virtual Task* task (void) = 0;

  int dt_count (void);

  void dt_ended (void);

  void check_ifexit (void);

 private:
  DT_LIST dt_list_;
  //Fixed_Priority_Scheduler* scheduler_;
  ACE_Barrier* barrier_;
  int dt_count_;
  CORBA::ORB_var orb_;
  /// Mutex to serialize access to our internal state.
  ACE_Lock* state_lock_;
  int active_dt_count_;
};


#endif /*DT_CREATOR_H*/
