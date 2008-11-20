// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: AppSideReg
 *
 *  @brief Declares interface for AppSideReg.
 *
 */

#ifndef __APPSIDEREG_H_
#define __APPSIDEREG_H_

#include <string>

#include "ace/Task.h"

#include "HostMonitorC.h"
#include "AppSideMonitor_Thread.h"

/**
 *  @class  AppSideReg
 *
 *  @brief Encapsulates AppSideReg
 */

class ACE_Barrier;

class LWFT_Server_Export AppSideReg : public ACE_Task_Base
{
public:
  AppSideReg (CORBA::ORB_ptr);

  ~AppSideReg (void);

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
  
  void unregister_process (void);

private:
  std::string HM_ior_;
  std::auto_ptr <AppSideMonitor_Thread> monitor_;
  HostMonitor_var hmvar_;
  CORBA::ORB_var orb_;
  ACE_Barrier outer_barrier_;
};



#endif /// __APPSIDEREG_H_
