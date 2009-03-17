#ifndef WORKER_I_H_
#define WORKER_I_H_

#include "WorkerS.h"
#include "CPU/CPU_Worker.h"
#include "ace/Task.h"
#include "ace/Condition_T.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/orbsvcs/LWFT/StateSynchronizationAgentC.h"

class Failure_Task : public ACE_Task_Base
{
 public:
  Failure_Task (CORBA::ORB_ptr orb,
                long limit,
                long & count);

  int svc (void);

  void signal (void);

  void stop (void);

 private:
  ACE_Thread_Mutex lock_;
  ACE_Condition <ACE_Thread_Mutex> condition_;
  CORBA::ORB_var orb_;
  long limit_;
  long & count_;
  bool stop_;
};

class Worker_i : public POA_DeCoRAM::Worker
{
 public:
  Worker_i (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa,
            const std::string & object_id,
	    StateSynchronizationAgent_ptr agent,
	    long invocations);

  virtual CORBA::ULong run_task (CORBA::Double execution_time);

  virtual void set_state (const CORBA::Any & state_value);

  virtual CORBA::Any *get_state ();

  virtual void stop (void);

  virtual StateSynchronizationAgent_ptr agent (void);

  virtual void agent (StateSynchronizationAgent_ptr agent);

  virtual char * object_id (void);
  
  virtual void object_id (const char * object_id);

 private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;

  std::string object_id_;

  StateSynchronizationAgent_var agent_;

  CUTS_CPU_Worker cpu_;

  CORBA::Double load_;
  
  long state_;

  long suicidal_count_;

  Failure_Task task_;

  ACE_High_Res_Timer timer_;
  
  ACE_Time_Value last_execution_time_;
};

#endif /* WORKER_I_H_ */
