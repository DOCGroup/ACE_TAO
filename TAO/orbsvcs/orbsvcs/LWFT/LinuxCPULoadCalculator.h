// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: LinuxCPULoadCalculator
 *
 *  @brief Declares interface for LinuxCPULoadCalculator.
 *
 */

#ifndef __LINUXCPULOADCALCULATOR_H_
#define __LINUXCPULOADCALCULATOR_H_

#include "ace/Thread_Mutex.h"

#include "CPULoadCalculator.h"
#include "host_monitor_export.h"

/**
 *  @class  LinuxCPULoadCalculator
 *
 *  @brief Encapsulates LinuxCPULoadCalculator
 */

class HostMonitor_Export LinuxCPULoadCalculator
  : public CPULoadCalculator
{
public:

    LinuxCPULoadCalculator (void);
    ~LinuxCPULoadCalculator (void);

    virtual double percent_load (void);

private:
    ACE_Thread_Mutex calc_mutex_;
};

#endif /// __LINUXCPULOADCALCULATOR_H_
