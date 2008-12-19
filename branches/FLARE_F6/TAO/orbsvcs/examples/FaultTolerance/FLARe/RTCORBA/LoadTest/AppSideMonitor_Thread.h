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

#include "AppSideMonitor_Handler.h"

/**
 *  @class  AppSideMonitor_Thread
 *
 *  @brief Encapsulates AppSideMonitor_Thread
 */

class AppSideMonitor_Thread : public ACE_Task_Base
{
public:

  AppSideMonitor_Thread (ACE_Barrier *thread_barrier);

  void stop ();
  virtual int svc (void);

private:
  u_short port_;
  ACE_SOCK_Acceptor::PEER_ADDR serv_addr_;
  ACE_Reactor reactor_;
  ACE_Acceptor <AppSideMonitor_Handler, ACE_SOCK_Acceptor> acceptor_;
  ACE_Barrier *synchronizer_;
};



#endif /// __APPSIDEMONITOR_THREAD_H_
