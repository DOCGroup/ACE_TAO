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

#include "ArgPair.h"

#include "AppSideReg.h"
#include "AppOptions.h"

AppSideReg::AppSideReg (ACE_Barrier *ext_barrier, CORBA::ORB_ptr orb)
  : HM_ior_ (AppOptions::instance ()->host_monitor_ior ()),
    orb_ (CORBA::ORB::_duplicate (orb)),
    external_barrier_ (ext_barrier)
{
}

AppSideReg::~AppSideReg (void)
{
  monitor_->stop ();
  orb_->destroy ();
}

void AppSideReg::unregister_process (void)
{
  hmvar_->unregister_process (
    AppOptions::instance ()->process_id ().c_str ());
}

int AppSideReg::svc (void)
{
  try
    {
      CORBA::Object_var obj = orb_->string_to_object (HM_ior_.c_str());

      if (CORBA::is_nil (obj))
        {
          ACE_DEBUG ((LM_ERROR, "Nil Reference\n"));
          return 1;
        }

      /// Downcast the object reference to a reference of type HostMonitor.
      this->hmvar_ = HostMonitor::_narrow (obj);
      
      if (CORBA::is_nil (hmvar_))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Argument is not a HostMonitor reference.\n"),
                            1);
        }

      //ACE_DEBUG ((LM_DEBUG, "Creating the host monitor\n"));

      ACE_Barrier internal_thread_barrier (2);
      monitor_ =
        std::auto_ptr<AppSideMonitor_Thread> (
          new AppSideMonitor_Thread (&internal_thread_barrier));
      monitor_->activate ();

      //ACE_DEBUG ((LM_DEBUG, "Host monitor activated\n"));

      internal_thread_barrier.wait ();
      
      /// Waiting for the AppSideMonitor_Thread to finish its socket stuff.
      try
        {
          hmvar_->dump ();
        }
      catch (CORBA::Exception &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "AppSideReg: exception from dump.\n"));
          throw;
        }

      //ACE_DEBUG ((LM_DEBUG, "AppSideReg::svc before registering process.\n"));

      //ACE_DEBUG ((LM_DEBUG, "Registering process\n"));
      try
        {
          if (hmvar_->register_process (
                AppOptions::instance ()->process_id ().c_str (),
                AppOptions::instance ()->host_id ().c_str (),
                AppOptions::instance ()->get_port ()))
            {
              //ACE_DEBUG ((LM_DEBUG,
              //            "Registered successfully %s with host monitor.\n",
              //            AppOptions::instance()->process_id().c_str()));
            }
          else
            {
              ACE_DEBUG ((LM_ERROR,
                          "Registeration with the monitor failed.\n"));
            }
        }
      catch (CORBA::Exception &)
        {
          ACE_DEBUG ((LM_DEBUG,
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

  //ACE_DEBUG ((LM_DEBUG, "AppSideReg::svc waiting on barrier.\n"));

  external_barrier_->wait ();
  return 0;
}

