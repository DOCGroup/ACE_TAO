/**
 *  @file  C++ Implementation: LinuxCPULoadCalculator
 *
 *  @brief Defines implementation of LinuxCPULoadCalculator.
 *
 */

#include "LinuxCPULoadCalculator.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"

LinuxCPULoadCalculator::LinuxCPULoadCalculator()
 : CPULoadCalculator()
{
}


LinuxCPULoadCalculator::~LinuxCPULoadCalculator()
{
}

double LinuxCPULoadCalculator::percent_load (void)
{
  ACE_Guard <ACE_Thread_Mutex> guard (calc_mutex_);
  //ACE_DEBUG((LM_DEBUG,"entering percent_load."));
  static char buf[1024];
  static unsigned long prev_idle = 0;
  static double prev_total = 0.0;

  FILE *file_ptr = 0;
  char *item = 0;
  char *arg = 0;
  unsigned long delta_idle = 0;
  unsigned long user = 0;
  unsigned long nice = 0;
  unsigned long idle = 0;
  unsigned long sys = 0;

  double percent_cpu_load = 0.0;

  if ((file_ptr = ACE_OS::fopen("/proc/stat", "r")) == 0)
          return percent_cpu_load;

  while ((ACE_OS::fgets (buf, sizeof (buf), file_ptr)) != 0)
  {
    item = ACE_OS::strtok (buf, " \t\n");
    arg = ACE_OS::strtok (0, "\n");

    if (item == 0 || arg == 0)
            continue;
    if (item[0] == 'c' && strlen (item) == 3)
    {
      sscanf (arg, "%lu %lu %lu %lu", &user, &nice, &sys, &idle);
      break;
    }
  }

  fclose (file_ptr);

  delta_idle = idle - prev_idle;
  double total;
  double time_passed;
  total = (double) (user + nice + sys + idle);
  time_passed = total - prev_total;

  percent_cpu_load = 100.0 - (delta_idle / time_passed * 100.0);

  prev_idle = idle;
  prev_total = total;

  //ACE_DEBUG((LM_DEBUG,"exiting percent_load."));
  return percent_cpu_load;
}

