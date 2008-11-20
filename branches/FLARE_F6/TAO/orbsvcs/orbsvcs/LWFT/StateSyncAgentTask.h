// -*- C++ -*-

//=============================================================================
/**
 *  @file    StateSyncAgentTask.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _STATE_SYNC_AGENT_TASK_H_
#define _STATE_SYNC_AGENT_TASK_H_

#include "ace/Task.h"
#include "ace/Barrier.h"
#include "StateSynchronizationAgent_i.h"
#include "ssa_export.h"

class SSA_Export StateSyncAgentTask : public ACE_Task_Base
{
public:
  StateSyncAgentTask (CORBA::ORB_ptr orb,
		                  StateSynchronizationAgent_i * agent);

  virtual int svc (void);
  
  // Override, calls base class activate(), then wait() on barrier.
  virtual int activate (long flags = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
                        int n_threads = 1,
                        int force_active = 0,
                        long priority = ACE_DEFAULT_THREAD_PRIORITY,
                        int grp_id = -1,
                        ACE_Task_Base *task = 0,
                        ACE_hthread_t thread_handles[] = 0,
                        void *stack[] = 0,
                        size_t stack_size[] = 0,
                        ACE_thread_t thread_ids[] = 0,
                        const char* thr_name[] = 0);

  StateSynchronizationAgent_ptr agent_ref (void);

 private:
  CORBA::ORB_var orb_;

  StateSynchronizationAgent_i * agent_;

  ACE_Barrier sync_;

  StateSynchronizationAgent_var agent_ref_;
};

#endif /* _STATE_SYNC_AGENT_TASK_H_ */
