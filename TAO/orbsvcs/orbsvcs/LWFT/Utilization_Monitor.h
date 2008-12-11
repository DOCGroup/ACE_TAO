// -*- C++ -*-
// $Id$

#ifndef UTILIZATION_MONITOR_H
#define UTILIZATION_MONITOR_H

#include <fstream>

#include "Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Monitor_Manager.h"
#include "host_monitor_export.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

class RM_Proxy;

class HostMonitor_Export Utilization_Monitor : protected Timer
{
public:
  using Timer::start;
  using Timer::stop;
  using Timer::hertz;

  Utilization_Monitor (void);
  virtual ~Utilization_Monitor (void);

  /// Helper function to be called back after a timeout.
  virtual int pulse (void);
  virtual double percent_load (void);
  void set_RM_Proxy (RM_Proxy *rm_proxy);

private:
  Monitor_Manager load_calc_;
  RM_Proxy *rm_proxy_;
  double load_;
  std::ofstream outfile_;
};

#endif /* UTILIZATION_MONITOR_H */
