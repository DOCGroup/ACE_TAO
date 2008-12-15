#ifndef WORKER_I_H_
#define WORKER_I_H_

#include "WorkerS.h"
#include "CPU/CPU_Worker.h"

#ifdef USE_STATESYNCHRONIZATION
#  include "orbsvcs/orbsvcs/LWFT/StateSynchronizationAgentC.h"
#endif

class Worker_i : public POA_DeCoRAM::Worker
{
 public:
  Worker_i (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa,
            const std::string & object_id);

  virtual void run_task (CORBA::Double execution_time);

  virtual void set_state (const CORBA::Any & state_value);

  virtual CORBA::Any *get_state ();
#ifdef USE_STATESYNCHRONIZATION
  virtual StateSynchronizationAgent_ptr agent (void);

  virtual void agent (StateSynchronizationAgent_ptr agent);
#endif  
  virtual char * object_id (void);
  
  virtual void object_id (const char * object_id);

 private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;

  std::string object_id_;
#ifdef USE_STATESYNCHRONIZATION
  StateSynchronizationAgent_var agent_;
#endif

  CUTS_CPU_Worker cpu_;

  CORBA::Double load_;
  
  long state_;

  long suicidal_count_;
};

#endif /* WORKER_I_H_ */
