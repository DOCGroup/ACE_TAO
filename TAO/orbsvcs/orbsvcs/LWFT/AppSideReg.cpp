// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideReg
 *
 *  @brief Defines implementation of AppSideReg.
 *
 */
#include "ace/Barrier.h"

#include "AppSideReg.h"
#include "AppOptions.h"

AppSideReg::AppSideReg (CORBA::ORB_ptr orb)
  : HM_ior_ (AppOptions::instance ()->host_monitor_ior ()),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

AppSideReg::~AppSideReg (void)
{
  // Now that this class is no longer a thread, the line below
  // will have to appear somewhere else.
//  monitor_->stop ();
}

int
AppSideReg::register_process (void)
{
  try
    {
      CORBA::Object_var obj = orb_->string_to_object (HM_ior_.c_str());

      if (CORBA::is_nil (obj))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideReg::svc: nil HM reference\n"),
                            1);
        }

      /// Downcast the object reference to a reference of type HostMonitor.
      this->hmvar_ = HostMonitor::_narrow (obj);
      
      if (CORBA::is_nil (hmvar_))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "AppSideReg::svc: HostMonitor "
                             "narrow failed.\n"),
                            1);
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
	            ACE_ERROR_RETURN ((LM_ERROR,
			                           "AppSideReg::svc: HostMonitor "
			                           "heartbeat_port () call failed.\n"),
			                          1);
	          }
	      }
      else
	      {
	        port_ = AppOptions::instance ()->port ();
	      }

      ACE_DEBUG ((LM_TRACE,
                  "AppSideReg: creating the host monitor\n"));

      ACE_Barrier internal_thread_barrier (2);
      AppSideMonitor_Thread *mon =
          new AppSideMonitor_Thread (internal_thread_barrier,
                                     port_);

      mon->activate ();

      //ACE_DEBUG ((LM_TRACE, "Host monitor activated\n"));

      internal_thread_barrier.wait ();
      
      // Store monitor in singleton so it will stay around.
      AppOptions::instance ()->monitor (mon);

      //ACE_DEBUG ((LM_TRACE, "AppSideReg::svc before registering process.\n"));

      try
        {
          //ACE_DEBUG ((LM_TRACE, "AppSideReg::svc - got heartbeat port %d from hm.\n", port_));
          CORBA::Boolean good_register =
            hmvar_->register_process (
              AppOptions::instance ()->process_id ().c_str (),
              AppOptions::instance ()->host_id ().c_str (),
              port_);
              
          if (good_register)
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
          throw;
        }

      //ACE_DEBUG ((LM_DEBUG, "Registering process\n"));
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "AppSideReg: A CORBA exception was raised:");
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, "AppSideReg: Unknown exception raised!"));
      return -1;
    }

  return 0;
}

void
AppSideReg::unregister_process (void)
{
  hmvar_->unregister_process (
    AppOptions::instance ()->process_id ().c_str ());
}

