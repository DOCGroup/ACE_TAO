// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideMonitor_Thread
 *
 *  @brief Defines implementation of AppSideMonitor_Thread.
 *
 */

#include "ace/TP_Reactor.h"

#include "tao/Object.h"

#include "Barrier_Guard.h"

#include "AppSideMonitor_Thread.h"
#include "AppOptions.h"

AppSideMonitor_Thread *
AppSideMonitor_Thread::instance (void)
{
  return
    ACE_Singleton<AppSideMonitor_Thread, ACE_SYNCH_MUTEX>::instance ();
}

AppSideMonitor_Thread::AppSideMonitor_Thread (void)
  : reactor_ (new ACE_TP_Reactor),
    acceptor_ (serv_addr_, &reactor_),
    sync_ (2),
    activated_ (false)
{
}

AppSideMonitor_Thread::~AppSideMonitor_Thread (void)
{
  this->stop ();
  this->wait ();
}

void
AppSideMonitor_Thread::stop (void)
{
  if (reactor_.end_reactor_event_loop() == -1)
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("end_reactor_event_loop failed\n")));
    }
}

int
AppSideMonitor_Thread::svc (void)
{
  {
    Barrier_Guard barrier_guard (sync_);
    
    try
      {
      try
        {
          AppOptions *opts = AppOptions::instance ();
          
          CORBA::Object_var obj =
            opts->orb ()->string_to_object (
              opts->host_monitor_ior ().c_str ());
              
          if (CORBA::is_nil (obj.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                                 ACE_TEXT ("nil HM reference\n")),
                                1);
            }

          hmvar_ = HostMonitor::_narrow (obj.in ());
            
          if (CORBA::is_nil (hmvar_.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                                 ACE_TEXT ("HostMonitor narrow failed.\n")),
                                1);
            }
        }    
      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (
            ex,
            ACE_TEXT ("AppSideMonitor_Thread::svc: ")
            ACE_TEXT ("HostMonitor resolve failed"));
          return -1;
        }
            
      // Retrieve the heartbeat port number from the HostMonitor
      // if no port parameter has been given.
      if (AppOptions::instance ()->port () == 0)
        {
          try
            {
              port_ = hmvar_->heartbeat_port ();
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (
                ex,
                ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                ACE_TEXT ("HostMonitor::heartbeat_port()"));
              return -1;
            }
        }
      else
        {
          port_ = AppOptions::instance ()->port ();
        }
      /*
      ACE_DEBUG ((LM_TRACE,
                  ACE_TEXT ("AppSideReg::svc - got heartbeat port ")
                  ACE_TEXT ("%d from hm.\n"),
                  port_));
      */
      if (serv_addr_.set (port_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                             ACE_TEXT ("can't set port.\n")),
                            -1);
        }
      
      if (acceptor_.open (serv_addr_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                             ACE_TEXT ("can't open the socket.\n")),
                            -1);
        }
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("AppSideMonitor_Thread::svc: ")
                         ACE_TEXT ("Unknown exception raised!")),
                        -1);
    }
  }

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Entering reactor event loop.\n")));
  if (reactor_.run_reactor_event_loop() == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("run_reactor_event_loop failed\n")),
                        -1);
    }
  
  return 0;
}

int
AppSideMonitor_Thread::activate (long /* flags */,
                                 int /* n_threads */,
                                 int /* force_active */,
                                 long /* priority */,
                                 int /* grp_id */,
                                 ACE_Task_Base* /* task */,
                                 ACE_hthread_t /* thread_handles */ [],
                                 void* /* stack */ [],
                                 size_t /* stack_size */ [],
                                 ACE_thread_t /* thread_ids */ [],
                                 const char* /* thr_name */ [])
{
  if (activated_)
    {
      return 0;
    }
    
  activated_ = true;
    
  // This will end up back in our overridden svc() method. We
  // want to wait for it to execute the statements in its body
  // before returning control to the calling application.
  int retval = this->ACE_Task_Base::activate ();
  sync_.wait ();
  
  if (retval != 0)
    {
      return retval;
    }
  
  AppOptions *app_opts = AppOptions::instance ();
  
  try
    {
      CORBA::Boolean good_register =
        hmvar_->register_process (app_opts->process_id ().c_str (),
                                  app_opts->host_id ().c_str (),
                                  port_);
          
      if (good_register)
        {
    /*
          ACE_DEBUG ((LM_TRACE,
                      ACE_TEXT ("Registered successfully %s ")
                      ACE_TEXT ("with host monitor.\n"),
                      AppOptions::instance ()->process_id ().c_str())); 
    */
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Registeration with the monitor ")
                             ACE_TEXT ("failed. Maybe the ")
                             ACE_TEXT ("hostmonitor needs to be set to ")
                             ACE_TEXT ("another ")
                             ACE_TEXT ("port range.\n")),
                            1);
        }
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (
        ex,
        ACE_TEXT ("AppSideMonitor_Thread::svc: ")
        ACE_TEXT ("HostMonitor::register_process():"));
      return -1;
    }

  return 0;
}

