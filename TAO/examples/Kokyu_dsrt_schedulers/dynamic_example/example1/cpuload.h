//$Id$
#ifndef CPULOAD_H
#define CPULOAD_H

#include <sys/time.h>
#include "ace/Log_Msg.h"

class CPULoad
{
 public:
  static void run (timeval& tv);
  static void CPULoad::calibrate (int num_iters = 10);
};

#endif
