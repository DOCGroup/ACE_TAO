
#include "Utilization_Monitor.h"
#include "HMOptions.h"

#include "ace/Log_Msg.h"

Utilization_Monitor::Utilization_Monitor (CPULoadCalculator *load_calc)
: load_calc_ (load_calc),
  load_ (0),
  outfile_ (HMOptions::instance()->util_file().c_str())
{
}

Utilization_Monitor::~Utilization_Monitor ()
{}

int
Utilization_Monitor::pulse (void)
{
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Utilization_Monitor::pulse\n")));
  load_ = load_calc_->percent_load ();
  outfile_ << load_ << std::endl;

  return 0;
}

double Utilization_Monitor::percent_load (void)
{
  return load_;
}

void Utilization_Monitor::setRM_Proxy (RM_Proxy *rm_proxy)
{
  this->rm_proxy_ = rm_proxy;
}
