// -*- C++ -*-
// $Id$

#ifndef FORWARDINGAGENT_THREAD_H
#define FORWARDINGAGENT_THREAD_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "ace/Barrier.h"

#include "tao/ORB_Core.h"

#include "lwft_client_export.h"

class ForwardingAgent_i;

class LWFT_Client_Export ForwardingAgent_Thread
  : public ACE_Task_Base
{
public:
  ForwardingAgent_Thread (void);
  ~ForwardingAgent_Thread (void);

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
                        
  void register_orb_initializer (void);
  void orb (CORBA::ORB_ptr the_orb);

private:
  CORBA::ORB_ptr orb_;

  ForwardingAgent_i *agent_;

  ACE_Barrier synchronizer_;
};

#include /**/ "ace/post.h"

#endif /* FORWARDINGAGENT_THREAD_H */
