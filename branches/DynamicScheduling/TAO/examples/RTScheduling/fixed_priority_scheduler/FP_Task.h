//$Id$
#ifndef FP_TASK_H
#define FP_TASK_H

#include "../Thread_Task.h"

class FP_Task : public Thread_Task
{
public:
  FP_Task (int importance,
	   int start_time,
	   int load,
	   DT_Creator *dt_creator);

  virtual int perform_task (void);
};

#endif /*FP_TASK_H*/
