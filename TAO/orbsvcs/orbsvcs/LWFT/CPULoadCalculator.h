// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: CPULoadCalculator
 *
 *  @brief Declares interface for CPULoadCalculator.
 *
 */

#ifndef __CPULOADCALCULATOR_H
#define __CPULOADCALCULATOR_H

#include "host_monitor_export.h"

class HostMonitor_Export CPULoadCalculator
{
  public:
    virtual double percent_load (void) = 0;
    virtual ~CPULoadCalculator (void) {}
};


#endif /// __CPULOADCALCULATOR_H
