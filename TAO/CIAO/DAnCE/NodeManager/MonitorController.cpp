// $Id$
//----------------------------------------------------------------------------------
/**
 * @file MonitorController.cpp
 *
 * @brief The Monitor Controller implementation.
 *
 * This is the facade class for Monitor
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//----------------------------------------------------------------------------------

#include "MonitorController.h"
#include "BaseMonitor.h"
#include "ace/DLL.h"
#include <ace/SString.h>
#include "MonitorCB.h"
#include "CIAO_common.h"

#include "ace/Log_Msg.h"

typedef MonitorBase* (*MonitorFactory) (void);

/// for the RSS Monitor
//const char* monitor_lib_name = "rssmon";

/// for the CIAO monitor
const char* monitor_lib_name = "ciaomonlib";

// The interval after which update will be sent.
// This value will sent by the EM in the later implementation
const int interval = 10;

static const char* factory_func = "createMonitor";

MonitorController::MonitorController (CORBA::ORB_ptr orb,
                                     ::Deployment::Domain& domain,
                                     ::Deployment::TargetManager_ptr target
                                     )
  : target_facet_i_ (::Deployment::TargetManager::_duplicate (target)),
    terminate_flag_ (0),
    orb_ (orb),
    initial_domain_ (domain)
{
}

int MonitorController::init()
{
  ACE_DEBUG ((LM_DEBUG , "Calling MonitorController Init\n"));

  ACE_DLL dll;

  // forming the library name
  ACE_CString lib_name = ACE_DLL_PREFIX;
  lib_name  += monitor_lib_name;

  int retval
    = dll.open (lib_name.c_str ());

  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                       "dll.open"),
                      -1);


  MonitorFactory factory =
    (MonitorFactory) dll.symbol (factory_func);

  if (factory == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p",
                      "dll.symbol"),
                      -1);
  {
    ACE_TRACE ((LM_DEBUG "Inside the init call\n"));

    monitor_.reset ((MonitorBase*) factory ());
    monitor_->initialize_params (initial_domain_,
                                 target_facet_i_.in (),
                                 interval);


    // Start the Monitor
    monitor_->start (orb_);
    std::auto_ptr <MonitorCB> monitor_callback (
                    new MonitorCB (orb_,
                                   target_facet_i_.in (),
                                   interval));

    // The loop in which UpdateData is called
    while (!terminating ())
    {
      // data will be updated in intervals of 10 secs.
      // in the latest version of spec , this value will
      // come from Execution Manager
      ACE_OS::sleep (interval);
//      ACE_DEBUG ((LM_DEBUG , "=The Terminate is %d\n", terminate_flag_));
      ::Deployment::Domain* domain =
                      monitor_->get_current_data ();

      monitor_callback->update_data (*domain);
    }
    monitor_->stop ();
  }

  dll.close ();

  return 0;

}


int MonitorController::svc (void)
{
  init ();
  if (CIAO::debug_level () > 9)
  {
    ACE_DEBUG ((LM_DEBUG , "Terminating Monitor\n"));
  }
  return 0;
}

MonitorController::~MonitorController ()
{
  terminate ();
  wait ();
}
