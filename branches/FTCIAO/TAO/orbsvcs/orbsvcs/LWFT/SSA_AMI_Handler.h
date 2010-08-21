#ifndef __SSA_AMI_HANDLER_H
#define __SSA_AMI_HANDLER_H

#include "StateSynchronizationAgentS.h"
#include "ssa_export.h"
#include <ace/Condition_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Time_Value.h>

class SSA_Export SSA_AMI_Handler 
  : public ::POA_AMI_StateSynchronizationAgentHandler
{
  private:
    size_t accumulated_results_;
    bool success_;
    ACE_Thread_Mutex ami_result_lock_; 
    //ACE_Thread_Mutex phase_wait_mutex_;
    //ACE_Condition<ACE_Thread_Mutex> phase_end_condition_;

    //void release();
  
  public:
    SSA_AMI_Handler();
    bool wait_for_results(size_t expected_results, ACE_Time_Value abstime);
    //void ssa_invoked();

    virtual void state_changed() {}
    virtual void state_changed_excep(Messaging::ExceptionHolder*) {}
    
    virtual void precommit_state(CORBA::Boolean);
    virtual void precommit_state_excep(Messaging::ExceptionHolder*);
     
    virtual void commit_state();
    virtual void commit_state_excep(Messaging::ExceptionHolder*);
     
    virtual void transfer_state();
    virtual void transfer_state_excep(Messaging::ExceptionHolder*);
     
    virtual void update_rank_list() {}
    virtual void update_rank_list_excep(Messaging::ExceptionHolder*) {}
     
    virtual void register_application() {}
    virtual void register_application_excep(Messaging::ExceptionHolder*) {}
};

#endif // __SSA_AMI_HANDLER_H


