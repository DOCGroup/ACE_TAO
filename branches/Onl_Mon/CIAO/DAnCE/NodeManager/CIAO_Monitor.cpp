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
#include "ace/Log_Msg.h"
#include <string>
#include <fstream>

using namespace std;

extern "C" ACE_Proper_Export_Flag CIAO::MonitorBase * CIAO::createMonitor ()
{
  CIAO::CIAO_Monitor* monitor = 0;
  ACE_NEW_RETURN (monitor, CIAO::CIAO_Monitor, 0);
  return monitor;
}

CIAO::CIAO_Monitor::CIAO_Monitor ()
{
  previous_.user_cpu = 0;
  previous_.user_cpu_low = 0;
  previous_.system_cpu = 0;
  previous_.idle_time = 0;
  previous_.total_load = 0;
}

/// The Desctructor
CIAO::CIAO_Monitor::~CIAO_Monitor ()
{
}

int CIAO::CIAO_Monitor::initialize_params (
                                     ::Deployment::Domain& domain,
                                     int interval
                                     )
{
  current_domain_.reset (new ::Deployment::Domain (domain));
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

  CORBA::Double current_load = calculate_load ();

  CORBA::Any any;
  any <<= current_load;

//   ACE_DEBUG ((LM_DEBUG, "(%P) CIAO_Monitor::get_current_data [%s]:"
//               " :-The current load is %f\n",
//               current_domain_->node[0].name.in (),
//               current_load));

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
                           "Current"))
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

void CIAO::CIAO_Monitor::set_context (MonitorController* context)
{
  this->controller_ = context;
}

double CIAO::CIAO_Monitor::calculate_load ()
{
  ifstream res_file;

  res_file.open ("/proc/stat");


  string cpu; // for getting the cpu value

  res_file >> cpu;
  res_file >> current_.user_cpu;
  res_file >> current_.user_cpu_low;
  res_file >> current_.system_cpu;
  res_file >> current_.idle_time;

  res_file.close ();

  current_.total_load = current_.user_cpu + current_.user_cpu_low +
                        current_.system_cpu + current_.idle_time;

  double load = current_.total_load - previous_.total_load;
  //  double user_current_load = (current_.user_cpu - previous_.user_cpu)/load;
  //  double system_current_load = (current_.system_cpu - previous_.system_cpu)/load;
  double delta_idle = current_.idle_time - previous_.idle_time;
  double cpu_load = 1.0 - (delta_idle / load);
  previous_ = current_;
  return cpu_load;
}
