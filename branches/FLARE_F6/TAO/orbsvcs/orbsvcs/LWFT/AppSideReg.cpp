// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideReg
 *
 *  @brief Defines implementation of AppSideReg.
 *
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "ace/Barrier.h"

#include "AppSideReg.h"
#include "AppOptions.h"

AppSideReg::AppSideReg (CORBA::ORB_ptr orb)
  : HM_ior_ (AppOptions::instance ()->host_monitor_ior ()),
    orb_ (CORBA::ORB::_duplicate (orb)),
    outer_barrier_ (2)
{
}

AppSideReg::~AppSideReg (void)
{
  monitor_->stop ();
  orb_->destroy ();
}

int
AppSideReg::svc (void)
{
  try
    {
      CORBA::Object_var obj = orb_->string_to_object (HM_ior_.c_str());

      if (CORBA::is_nil (obj))
        {
          ACE_DEBUG ((LM_ERROR, \
                      "AppSideReg::svc: nil HM reference\n"));
          outer_barrier_.wait ();
          return 1;
        }

      /// Downcast the object reference to a reference of type HostMonitor.
      this->hmvar_ = HostMonitor::_narrow (obj);
      
      if (CORBA::is_nil (hmvar_))
        {
          ACE_ERROR ((LM_ERROR,
                      "AppSideReg::svc: HostMonitor narrow failed.\n"));
          outer_barrier_.wait ();
          return 1;
        }

      // retrieve the heartbeat port number from the HostMonitor if no
      // port parameter has been given.
      if (AppOptions::instance ()->port () == 0)
	      {
	        try
	          {
	            port_ = hmvar_->heartbeat_port ();
	          }
	        catch (CORBA::SystemException &)
	          {
	            ACE_ERROR ((LM_ERROR,
			        "AppSideReg::svc: HostMonitor "
			        "heartbeat_port () call failed.\n"));
	            outer_barrier_.wait ();
	            return 1;
	          }
	      }
      else
	      {
	        port_ = AppOptions::instance ()->port ();
	      }

      ACE_DEBUG ((LM_TRACE, "Creating the host monitor\n"));

      ACE_Barrier internal_thread_barrier (2);
      monitor_ =
        std::auto_ptr<AppSideMonitor_Thread> (
          new AppSideMonitor_Thread (internal_thread_barrier,
                                     port_));

      monitor_->activate ();

      //ACE_DEBUG ((LM_TRACE, "Host monitor activated\n"));

      internal_thread_barrier.wait ();
      
      /// Waiting for the AppSideMonitor_Thread to finish its socket stuff.
      try
        {
         // hmvar_->dump ();
        }
      catch (CORBA::Exception &)
        {
          ACE_DEBUG ((LM_ERROR,
                      "AppSideReg: exception from dump.\n"));
          outer_barrier_.wait ();
          throw;
        }

      //ACE_DEBUG ((LM_TRACE, "AppSideReg::svc before registering process.\n"));

      try
        {
          //ACE_DEBUG ((LM_TRACE, "AppSideReg::svc - got heartbeat port %d from hm.\n", port_));

          if (hmvar_->register_process (
                AppOptions::instance ()->process_id ().c_str (),
                AppOptions::instance ()->host_id ().c_str (),
                port_))
            {
	      /*
              ACE_DEBUG ((LM_TRACE,
                          "Registered successfully %s with host monitor.\n",
                          AppOptions::instance()->process_id().c_str())); 
	      */
            }
          else
            {
              ACE_DEBUG ((LM_ERROR,
                          "Registeration with the monitor failed. Maybe the "
                          "hostmonitor needs to be set to another "
                          "port range.\n"));
            }
        }
      catch (CORBA::Exception &)
        {
          ACE_DEBUG ((LM_ERROR,
                      "AppSideReg: exception from register_process.\n"));
          outer_barrier_.wait ();
          throw;
        }

      //ACE_DEBUG ((LM_DEBUG, "Registering process\n"));
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "AppSideReg: A CORBA exception was raised:");
      outer_barrier_.wait ();
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, "AppSideReg: Unknown exception raised!"));
      outer_barrier_.wait ();
      return -1;
    }

  //ACE_DEBUG ((LM_DEBUG, "AppSideReg::svc waiting on barrier.\n"));

  outer_barrier_.wait ();
  return 0;
}

int
AppSideReg::activate (long /* flags */,
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
  // This will end up back in our overridden svc() method. We
  // want to wait for it to execute the statements in its body
  // before returning control to the calling application.
  int retval = this->ACE_Task_Base::activate ();
  outer_barrier_.wait ();
  return retval;
}

void
AppSideReg::unregister_process (void)
{
  hmvar_->unregister_process (
    AppOptions::instance ()->process_id ().c_str ());
}

