// -*- C++ -*-
// $Id$

#include "ace/Log_Msg.h"
#include "ace/Monitor_Control/Monitor_Control.h"

#include "Utilization_Monitor.h"
#include "HMOptions.h"

Utilization_Monitor::Utilization_Monitor (void)
: load_ (0.0),
  outfile_ (HMOptions::instance ()->util_file ().c_str ())
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  load_calc_ = create_os_monitor<CPU_LOAD_MONITOR> ();

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */
}

Utilization_Monitor::~Utilization_Monitor (void)
{
}

int
Utilization_Monitor::pulse (void)
{
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Utilization_Monitor::pulse\n")));
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  load_calc_->update ();
  Monitor_Control_Types::Data data (load_calc_->type ());
  load_calc_->retrieve (data);
  load_ = data.value_;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  outfile_ << load_ << std::endl;

  return 0;
}

double Utilization_Monitor::percent_load (void)
{
  return load_;
}

void Utilization_Monitor::set_RM_Proxy (RM_Proxy *rm_proxy)
{
  this->rm_proxy_ = rm_proxy;
}
