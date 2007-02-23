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

extern "C" ACE_Proper_Export_Flag CIAO::MonitorBase * CIAO::createMonitor ()
{
  CIAO::CIAO_Monitor* monitor = 0;
  ACE_NEW_RETURN (monitor, CIAO::CIAO_Monitor, 0);
  return monitor;
}

CIAO::CIAO_Monitor::CIAO_Monitor ()
{
}

/// The Desctructor
CIAO::CIAO_Monitor::~CIAO_Monitor ()
{
}

int CIAO::CIAO_Monitor::initialize_params (
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

int CIAO::CIAO_Monitor::start (CORBA::ORB_ptr)
{
  return 0;
}

int CIAO::CIAO_Monitor::stop ()
{
  return 0;
}

::Deployment::Domain* CIAO::CIAO_Monitor::get_current_data ()
{
  if (CIAO::debug_level () > 9)
    {
      // ACE_DEBUG ((LM_DEBUG ,
      //            "CIAO_Monitor::Inside the get_current_data of[%s]\n",
      //            current_domain_->node[0].name.in ()));
    }

  CORBA::Double current_load = 0;

  long user_cpu;
  long user_cpu_low;
  long sys_cpu;
  long idle_time;

  // get the load average value from the /proc/loadavg

  FILE *load_file = 0;

  load_file = ACE_OS::fopen ("/proc/stat", "r");

  if (load_file == 0)
    {
      // load file cannot be opened ..
      current_load = 0;
    }
  else
    {
      char buffer [99];

      // read in the cpu label
      fscanf (load_file, "%s", buffer);

      //read the user_cpu
      fscanf (load_file, "%ld", &user_cpu);

      //read the user cpu low priority
      fscanf (load_file, "%ld", &user_cpu_low);

      //read the system cpu
      fscanf (load_file, "%ld", &sys_cpu);

      //read the cpu in idle time ..
      fscanf (load_file, "%ld", &idle_time);

      ACE_OS::fclose (load_file);

      // Calculate the percent CPU
      long const current_user_cpu = user_cpu - prev_user_cpu_;
      long const total_cpu_usage = user_cpu + user_cpu_low + sys_cpu +
        idle_time - prev_user_cpu_ - prev_idle_time_ - prev_sys_cpu_
        - prev_user_cpu_low_;

      current_load = (current_user_cpu * 100)/total_cpu_usage;

      // Save the current cpu values in the previous variables

      prev_user_cpu_ = user_cpu;

      prev_user_cpu_low_ = user_cpu_low;

      prev_sys_cpu_ = sys_cpu;

      prev_idle_time_ = idle_time;

    }


  CORBA::Any any;
  any <<= current_load;

  // here insert the util value, in the right position

  for (unsigned int i = 0;
       i < current_domain_->node[0].resource.length ();
       i++)
    {
      if (!ACE_OS::strcmp (current_domain_->node[0].resource[i].name, "Processor"))
        {
          for (unsigned int j = 0;
               j < current_domain_->node[0].resource[i].property.length ();
               j++)
            {
              if (!ACE_OS::strcmp (
                           current_domain_
                           ->node[0].resource[i].property[j].name.in (),
                           "LoadAverage"))
                {
                  current_domain_->node[0].resource[i].property[j].kind =
                    ::Deployment::Quantity;
                  current_domain_->node[0].resource[i].property[j].value =
                    any;
                }
            }
        }
    }

  return current_domain_.get ();
}
