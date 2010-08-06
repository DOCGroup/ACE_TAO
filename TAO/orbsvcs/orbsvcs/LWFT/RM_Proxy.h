// -*- C++ -*-
// $Id$

#ifndef RM_PROXY_H
#define RM_PROXY_H

#include <string>

#include "Timer.h"

#include "ReplicationManagerC.h"

#include "Utilization_Monitor.h"

class HostMonitor_Export RM_Proxy : protected Timer
{
public:
  using Timer::start;
  using Timer::stop;
  using Timer::hertz;

  RM_Proxy (CORBA::ORB_ptr);
  virtual ~RM_Proxy (void);
  void set_Utilization_Monitor (Utilization_Monitor *load_calc);
  void proc_failure (const std::string &process_id);

  /// Helper function to be called back after a timeout
  virtual int pulse (void);

protected:
  CORBA::Object_ptr obtain_RM_ior (CORBA::ORB_ptr orb);

private:
  Utilization_Monitor *load_calc_;
  ReplicationManager_var RM_var_;
  CORBA::ORB_var orb_;
};


#endif /* RM_TIMER_H */
