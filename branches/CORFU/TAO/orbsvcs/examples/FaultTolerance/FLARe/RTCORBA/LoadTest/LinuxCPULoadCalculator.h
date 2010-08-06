/**
 *  @file  C++ Interface: LinuxCPULoadCalculator
 *
 *  @brief Declares interface for LinuxCPULoadCalculator.
 *
 */

#ifndef __LINUXCPULOADCALCULATOR_H_
#define __LINUXCPULOADCALCULATOR_H_

#include "CPULoadCalculator.h"
#include "ace/Thread_Mutex.h"

/**
 *  @class  LinuxCPULoadCalculator
 *
 *  @brief Encapsulates LinuxCPULoadCalculator
 */

class LinuxCPULoadCalculator : public CPULoadCalculator
{
public:

    LinuxCPULoadCalculator();

    virtual double percent_load (void);

    ~LinuxCPULoadCalculator();
private:
    ACE_Thread_Mutex calc_mutex_;

};

#endif /// __LINUXCPULOADCALCULATOR_H_
