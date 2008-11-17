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
		                  StateSynchronizationAgent_i * agent,
		                  ACE_Barrier * sync);

  int svc (void);

  StateSynchronizationAgent_ptr agent_ref (void);

 private:
  CORBA::ORB_var orb_;

  StateSynchronizationAgent_i * agent_;

  ACE_Barrier * sync_;

  StateSynchronizationAgent_var agent_ref_;
};

#endif /* _STATE_SYNC_AGENT_TASK_H_ */
