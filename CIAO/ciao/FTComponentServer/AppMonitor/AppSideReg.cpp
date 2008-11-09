/**
 *  @file  C++ Implementation: AppSideReg
 *
 *  @brief Defines implementation of AppSideReg.
 *
 */

#include "AppSideReg.h"
#include "monitorC.h"
#include "ace/Barrier.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "Name_Helper_T.h"

AppSideReg::AppSideReg(ACE_Barrier *ext_barrier, CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    external_barrier_ (ext_barrier)
{
}


AppSideReg::~AppSideReg()
{
  monitor_->stop ();
  orb_->destroy ();
}

void AppSideReg::unregister_process (void)
{
  hmvar_->unregister_process (this->get_process_id ().c_str ());
}

int AppSideReg::svc(void)
{
  try
  {
    Name_Helper_T <HostMonitor> hmh (orb_.in (), true);

    hmvar_ = hmh.resolve ("FLARe/" + 
			  hmh.escape_dots (this->get_hostname ()) + 
			  "/HostMonitor");

    u_short port = hmvar_->heartbeat_port (this->get_process_id ().c_str ());
    
    ACE_Barrier internal_thread_barrier (2);
    monitor_ = std::auto_ptr <AppSideMonitor_Thread> 
      (new AppSideMonitor_Thread (&internal_thread_barrier,
				  port));
    monitor_->activate ();
    
    //ACE_DEBUG ((LM_DEBUG, "Monitor activated\n"));
    
    internal_thread_barrier.wait ();
      
    /// Waiting for the AppSideMonitor_Thread to finish its socket stuff.
    try
      {
        hmvar_->dump ();
      }
    catch (CORBA::Exception &)
      {
        ACE_DEBUG ((LM_DEBUG,"exception from dump.\n"));
        throw;
      }

    try
      {
        if (hmvar_->register_process (
              this->get_process_id ().c_str (),
              this->get_hostname ().c_str (),
              port))
          {
          }
        else
          {
            ACE_DEBUG ((LM_ERROR, "Registeration with the monitor failed.\n"));
          }
      }
    catch (CORBA::Exception &)
      {
	ACE_DEBUG ((LM_DEBUG,"exception from register_process.\n"));
	throw;
      }


    //ACE_DEBUG ((LM_DEBUG, "Registering process\n"));
  }
  catch (CORBA::Exception &ex)
  {
    ACE_PRINT_EXCEPTION (ex, "AppSideReg::svc -  CORBA exception was raised:");
    return -1;
  }
  catch (Name_Helper_Exception & ex)
    {
      ACE_ERROR ((LM_ERROR, "AppSideReg::svc - "
		   "Name helper exception: %s", ex.what ()));
    }
  catch (...)
  {
    ACE_DEBUG((LM_ERROR, "AppSideReg::svc - Unknown exception raised!"));
    return -1;
  }

  //ACE_DEBUG ((LM_DEBUG, "AppSideReg::svc waiting on barrier.\n"));

  external_barrier_->wait ();
  return 0;
}

std::string
AppSideReg::get_hostname ()
{
  char hn_str [100];
  gethostname (hn_str, sizeof (hn_str));
  
  return std::string (hn_str);
}

std::string
AppSideReg::get_process_id ()
{
  pid_t pid = ACE_OS::getpid ();
  std::stringstream ss;
  ss << pid;
  
  return ss.str ();
}
