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
	   int iter,
	   int dist,
	   char *job_name,
	   DT_Creator *dt_creator);

  virtual int perform_task (void);

  virtual void pre_activate (void);

};

#endif /*FP_TASK_H*/
