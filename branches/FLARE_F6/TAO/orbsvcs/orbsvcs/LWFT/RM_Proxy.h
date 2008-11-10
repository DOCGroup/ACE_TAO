// -*- C++ -*-
// $Id$

#ifndef RM_PROXY_H
#define RM_PROXY_H

#include <string>

#include "Timer.h"

#include "ReplicationManagerC.h"

#include "CPULoadCalculator.h"

class HostMonitor_Export RM_Proxy : protected Timer
{
public:
  using Timer::start;
  using Timer::stop;
  using Timer::hertz;

  RM_Proxy (CORBA::ORB_ptr);
  virtual ~RM_Proxy (void);
  void setCPULoadCalculator (CPULoadCalculator *load_calc);
  void proc_failure (const std::string &process_id);

  /// Helper function to be called back after a timeout
  virtual int pulse (void);

protected:
  CORBA::Object_var obtain_RM_ior (CORBA::ORB_ptr orb);

private:
  CPULoadCalculator *load_calc_;
  ReplicationManager_var RM_var_;
  CORBA::ORB_var orb_;
};


#endif /* RM_TIMER_H */
