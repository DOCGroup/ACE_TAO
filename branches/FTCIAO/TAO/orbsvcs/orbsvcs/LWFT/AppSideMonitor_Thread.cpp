// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideMonitor_Thread
 *
 *  @brief Defines implementation of AppSideMonitor_Thread.
 *
 */

#include "ace/TP_Reactor.h"

#include "Barrier_Guard.h"

#include "AppSideMonitor_Thread.h"
#include "AppOptions.h"

AppSideMonitor_Thread::AppSideMonitor_Thread (
  ACE_Barrier &thread_barrier)
  : port_ (AppOptions::instance ()->port ()),
    reactor_ (new ACE_TP_Reactor),
    acceptor_ (serv_addr_, &reactor_),
    synchronizer_ (thread_barrier)
{
}

AppSideMonitor_Thread::AppSideMonitor_Thread (
  ACE_Barrier &thread_barrier,
  u_short port)
  : port_ (port),
    reactor_ (new ACE_TP_Reactor),
    acceptor_ (serv_addr_, &reactor_),
    synchronizer_ (thread_barrier)
{
}

int
AppSideMonitor_Thread::svc (void)
{
  {
    Barrier_Guard barrier_guard (synchronizer_);

    if (serv_addr_.set (this->port_) == -1)
      {
        ACE_DEBUG ((LM_ERROR,
                    "AppSideMonitor_Thread::svc: can't set port.\n"));
//      this->synchronizer_->wait ();
        return EXIT_FAILURE;
      }
    
    if (acceptor_.open (serv_addr_) == -1)
      {
        ACE_DEBUG ((LM_ERROR,
                    "AppSideMonitor_Thread::svc: can't open the socket.\n"));
//      this->synchronizer_->wait ();
        return EXIT_FAILURE;
      }

  } // release synchronizer here

//  this->synchronizer_->wait ();
//  this->synchronizer_ = 0;

  //ACE_DEBUG ((LM_DEBUG, "Entering reactor event loop.\n"));
  if (reactor_.run_reactor_event_loop() == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "run_reactor_event_loop failed\n"),
                        -1);
    }
  
  return 0;
}

void
AppSideMonitor_Thread::stop (void)
{
  if (reactor_.end_reactor_event_loop() == -1)
    {
      ACE_DEBUG ((LM_ERROR,
                  "end_reactor_event_loop failed\n"));
    }
}
