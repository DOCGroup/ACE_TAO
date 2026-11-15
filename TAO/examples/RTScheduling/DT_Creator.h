#ifndef DT_CREATOR_H
#define DT_CREATOR_H

#include "POA_Holder.h"
#include "Job_i.h"
#include "Synch_i.h"
#include "rtschedtestlib_export.h"

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/PortableServer/Servant_Base.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/Lock.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include <memory>
#include <string>
#include <vector>

class Thread_Task;
class Task;

class RTSCHEDTESTLIB_Export DT_Creator : public ACE_Service_Object
{
 public:
  DT_Creator ();

  ~DT_Creator () override;

  int init (int argc, ACE_TCHAR *argv []) override;

  int dt_task_init (ACE_Arg_Shifter& arg_shifter);

  virtual void create_distributable_threads (RTScheduling::Current_ptr  current);

  void activate_poa_list ();
  void activate_job_list ();
  void activate_schedule ();

  virtual void yield (time_t suspend_time,
                      Thread_Task* task) = 0;

  virtual void wait () = 0;

  virtual CORBA::Policy_ptr sched_param (int importance) = 0;

  virtual Thread_Task* create_thr_task (int importance,
          time_t start_time,
          int load,
          int iter,
          int dist,
          char *job_name) = 0;
  //  virtual Task* task () = 0;

  /// Resolve the naming service.
  int resolve_naming_service ();

  int dt_count ();

  void dt_ended ();
  void job_ended ();

  void check_ifexit ();

  void log_msg (std::string msg);

  void orb (CORBA::ORB_ptr);
  CORBA::ORB_ptr orb ();

  ACE_Time_Value* base_time ();
  void base_time (ACE_Time_Value*);

  ACE_hrtime_t base_hr_time ();

  virtual int total_load () = 0;

  Synch_i* synch ();

  void register_synch_obj ();

  int activate_root_poa ();

 protected:
  std::vector<std::unique_ptr<Thread_Task>> dt_list_ {};
  std::vector<std::unique_ptr<POA_Holder>> poa_list_ {};
  std::vector<PortableServer::ServantBase_var> job_list_ {};
  //Fixed_Priority_Scheduler* scheduler_;
  CORBA::ORB_var orb_;
  /// Mutex to serialize access to our internal state.
  std::unique_ptr<ACE_Lock> state_lock_ {};
  std::unique_ptr<ACE_Lock> shutdown_lock_ {};
  int active_dt_count_;
  int active_job_count_;
  std::vector<std::string> log_ {};
  std::unique_ptr<ACE_Time_Value> base_time_ {};
  /// A naming context.
  CosNaming::NamingContextExt_var naming_;
  ACE_TString file_name_;
  ACE_TString log_file_name_;
  ACE_High_Res_Timer::global_scale_factor_type gsf_;
  PortableServer::Servant_var<Synch_i> synch_;
};

#endif /*DT_CREATOR_H*/
