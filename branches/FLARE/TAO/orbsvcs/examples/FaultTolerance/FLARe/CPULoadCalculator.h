/**
 *  @file  C++ Interface: CPULoadCalculator
 *
 *  @brief Declares interface for CPULoadCalculator.
 *
 */

#ifndef __CPULOADCALCULATOR_H
#define __CPULOADCALCULATOR_H

class CPULoadCalculator
{
  public:
    virtual double percent_load (void) = 0;
    virtual ~CPULoadCalculator () {}
};


#endif /// __CPULOADCALCULATOR_H
