// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Implementation: AppSideReg
 *
 *  @brief Defines implementation of AppSideReg.
 *
 */

#include "AppSideReg.h"
#include "AppSideMonitor_Thread.h"

AppSideReg::AppSideReg (void)
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
  AppSideMonitor_Thread *mon =
    AppSideMonitor_Thread::instance ();
    
  int result = mon->activate ();
  
  if (result != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "AppSideReg::register_process: "
                         "monitor activation failed\n"),
                        -1);
    }

  //ACE_DEBUG ((LM_TRACE, "Host monitor activated\n"));

  return 0;
}
/*
void
AppSideReg::unregister_process (void)
{
  hmvar_->unregister_process (
    AppOptions::instance ()->process_id ().c_str ());
}
*/
