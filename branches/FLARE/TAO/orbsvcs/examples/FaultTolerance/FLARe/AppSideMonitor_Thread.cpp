/**
 *  @file  C++ Implementation: AppSideMonitor_Thread
 *
 *  @brief Defines implementation of AppSideMonitor_Thread.
 *
 */

#include "AppSideMonitor_Thread.h"
#include "AppOptions.h"

#include "ace/TP_Reactor.h"

AppSideMonitor_Thread::AppSideMonitor_Thread (ACE_Barrier *thread_barrier)
: port_ (AppOptions::instance()->get_port()),
  reactor_ (new ACE_TP_Reactor),
  acceptor_ (serv_addr_, &reactor_),
  synchronizer_ (thread_barrier)
{
}

int AppSideMonitor_Thread::svc (void)
{
  if (serv_addr_.set (this->port_, INADDR_ANY) == -1)
    {
      ACE_DEBUG ((LM_ERROR, "Can't set port.\n"));
      return EXIT_FAILURE;
    }
  if (acceptor_.open (serv_addr_) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "The Acceptor can't open the socket.\n"));
      return EXIT_FAILURE;
    }

  this->synchronizer_->wait ();
  this->synchronizer_ = 0;

  //ACE_DEBUG ((LM_DEBUG, "Entering reactor event loop.\n"));
  if (reactor_.run_reactor_event_loop() == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"run_reactor_event_loop failed\n"), -1);

  return 0;
}

void AppSideMonitor_Thread::stop ()
{
  if (reactor_.end_reactor_event_loop() == -1)
     ACE_DEBUG((LM_ERROR,"end_reactor_event_loop failed\n"));
}
