// $Id$
//==========================================================================
/// The RSS Monitor class defination
/**
 *  @file CIAO_Monitor.cpp
 *
 *  @brief The Monitor class defination
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//==========================================================================

#include "CIAO_Monitor.h"
#include "CIAO_common.h"
#include "ace/OS_NS_stdio.h"


extern "C" ACE_Proper_Export_Flag MonitorBase * createMonitor ()
{
  return new CIAO_Monitor ();
}

CIAO_Monitor::CIAO_Monitor ()
{
}

/// The Desctructor
CIAO_Monitor::~CIAO_Monitor ()
{
}

int  CIAO_Monitor::initialize_params (
                                     ::Deployment::Domain& domain,
                                     ::Deployment::TargetManager_ptr target_manager,
                                     int interval
                                     )
{
  current_domain_.reset (new ::Deployment::Domain (domain));
  target_ptr_= target_manager;
  this->interval_=interval;

  return 0;
}

int CIAO_Monitor::start (CORBA::ORB_ptr)
{
  return 0;
}

int CIAO_Monitor::stop ()
{
  return 0;
}

::Deployment::Domain* CIAO_Monitor::get_current_data ()
{
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Inside the get_current_data"));
    }

  float current_load;

  // get the load average value from the /proc/loadavg

  FILE *load_file = ACE_OS::fopen ("/proc/loadavg", "r");
  
  if (load_file == 0)
    {
      // load file cannot be opened ..
      current_load = 0;
    }
  else
    {
      fscanf (load_file, "%f", &current_load);
      if (CIAO::debug_level () > 9)
      {
        ACE_DEBUG ((LM_DEBUG , "Current load is %d\n",current_load));
      }
    }

  ACE_OS::fclose (load_file);

    CORBA::Any any;
    any <<= current_load;

    current_domain_->node[0].resource[0].property[0].kind = ::Deployment::Quantity;
    current_domain_->node[0].resource[0].property[0].value = any;

    if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Exiting from the get_current_data function\n"));
    }

    return current_domain_.get ();
}
