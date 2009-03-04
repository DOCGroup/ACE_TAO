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
#include "HostMonitorC.h"

AppSideMonitor_Thread*
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

void
AppSideMonitor_Thread::stop (void)
{
  if (reactor_.end_reactor_event_loop() == -1)
    {
      ACE_DEBUG ((LM_ERROR,
                  "end_reactor_event_loop failed\n"));
    }
}

int
AppSideMonitor_Thread::svc (void)
{
  {
    Barrier_Guard barrier_guard (sync_);
    
    HostMonitor_var hmvar;

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
                                 "AppSideMonitor_Thread::svc: "
                                 "nil HM reference\n"),
                                1);
            }

          hmvar = HostMonitor::_narrow (obj.in ());
            
          if (CORBA::is_nil (hmvar.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "AppSideMonitor_Thread::svc: "
                                 "HostMonitor narrow failed.\n"),
                                1);
            }
        }    
      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex,
                               "AppSideMonitor_Thread::svc: "
                               "HostMonitor resolve failed");
          return -1;
        }
            
      // Retrieve the heartbeat port number from the HostMonitor
      // if no port parameter has been given.
      if (AppOptions::instance ()->port () == 0)
        {
          try
            {
              port_ = hmvar->heartbeat_port ();
            }
          catch (CORBA::Exception &ex)
            {
              ACE_PRINT_EXCEPTION (ex,
                                   "AppSideMonitor_Thread::svc: "
                                   "HostMonitor::heartbeat_port()");
              return -1;
            }
        }
      else
        {
          port_ = AppOptions::instance ()->port ();
        }

      try
        {
//          ACE_DEBUG ((LM_TRACE,
//                      "AppSideReg::svc - got heartbeat port "
//                      "%d from hm.\n", port_));
          CORBA::Boolean good_register =
            hmvar->register_process (
              AppOptions::instance ()->process_id ().c_str (),
              AppOptions::instance ()->host_id ().c_str (),
              port_);
              
          if (good_register)
            {
        /*
              ACE_DEBUG ((LM_TRACE,
                          "Registered successfully %s "
                          "with host monitor.\n",
                          AppOptions::instance ()->process_id ().c_str())); 
        */
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Registeration with the monitor "
                                 "failed. Maybe the "
                                 "hostmonitor needs to be set to "
                                 "another "
                                 "port range.\n"),
                                1);
            }
        }
      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex,
                               "AppSideMonitor_Thread::svc: "
                               "HostMonitor::register_process():");
          return -1;
        }

      if (serv_addr_.set (port_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideMonitor_Thread::svc: "
                             "can't set port.\n"),
                            -1);
        }
      
      if (acceptor_.open (serv_addr_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideMonitor_Thread::svc: "
                             "can't open the socket.\n"),
                            -1);
        }
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "AppSideMonitor_Thread::svc: "
                         "Unknown exception raised!"),
                        -1);
    }
  }

  //ACE_DEBUG ((LM_DEBUG, "Entering reactor event loop.\n"));
  if (reactor_.run_reactor_event_loop() == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "run_reactor_event_loop failed\n"),
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
  else
    {
      activated_ = true;
    }
    
  // This will end up back in our overridden svc() method. We
  // want to wait for it to execute the statements in its body
  // before returning control to the calling application.
  int retval = this->ACE_Task_Base::activate ();
  sync_.wait ();
  
  // Keep a reference to the monitor singleton.
  AppOptions::instance ()->monitor (this);
  
  return retval;
}

