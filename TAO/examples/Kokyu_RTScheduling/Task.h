//$Id$
#ifndef TASK_H
#define TASK_H

#include "Task_Stats.h"
#include "tao/RTScheduling/RTScheduler.h"

class Task
{
public:
  virtual int perform_task (int load,
			    int thr_id) = 0;
};

#endif /*TASK_H*/
