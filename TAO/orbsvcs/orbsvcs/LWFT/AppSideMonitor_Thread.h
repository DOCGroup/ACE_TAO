// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: AppSideMonitor_Thread
 *
 *  @brief Declares interface for AppSideMonitor_Thread.
 *
 */

#ifndef __APPSIDEMONITOR_THREAD_H_
#define __APPSIDEMONITOR_THREAD_H_

#include "ace/Task.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Barrier.h"
#include "ace/Acceptor.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"

#include "AppSideMonitor_Handler.h"
#include "HostMonitorC.h"

/**
 *  @class  AppSideMonitor_Thread
 *
 *  @brief Encapsulates AppSideMonitor_Thread
 */

class LWFT_Server_Export AppSideMonitor_Thread : public ACE_Task_Base
{
public:
  friend class ACE_Singleton<AppSideMonitor_Thread, ACE_SYNCH_MUTEX>;

  /// Used to help ensure that there is only a single instance
  /// per process of AppSideMonitor_Thread.
  static AppSideMonitor_Thread* instance (void);

  void stop (void);
  
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

private:
  // Prevent instance creation outside of ACE_Singleton.
  AppSideMonitor_Thread (void);
  
private:
  u_short port_;
  ACE_SOCK_Acceptor::PEER_ADDR serv_addr_;
  ACE_Reactor reactor_;
  ACE_Acceptor<AppSideMonitor_Handler, ACE_SOCK_Acceptor> acceptor_;
  ACE_Barrier sync_;
  bool activated_;
  HostMonitor_var hmvar_;
};



#endif /// __APPSIDEMONITOR_THREAD_H_
