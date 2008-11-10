// -*- C++ -*-
// $Id$

#ifndef UTILIZATION_MONITOR_H
#define UTILIZATION_MONITOR_H

#include <fstream>

#include "Timer.h"

#include "CPULoadCalculator.h"

class RM_Proxy;

class HostMonitor_Export Utilization_Monitor
  : protected Timer,
    public CPULoadCalculator
{
public:
  using Timer::start;
  using Timer::stop;
  using Timer::hertz;

  explicit Utilization_Monitor (CPULoadCalculator *load_calc);
  virtual ~Utilization_Monitor (void);

  /// Helper function to be called back after a timeout
  virtual int pulse (void);
  virtual double percent_load (void);
  void setRM_Proxy (RM_Proxy *rm_proxy);

private:
  std::auto_ptr <CPULoadCalculator> load_calc_;
  RM_Proxy *rm_proxy_;
  double load_;
  std::ofstream outfile_;
};

#endif /* UTILIZATION_MONITOR_H */
