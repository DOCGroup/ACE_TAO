//$Id$
#ifndef FP_TASK_H
#define FP_TASK_H

#include "../Task.h"

class FP_Task : public Task
{
public:
  virtual int perform_task (int load,
			    int thr_id);
};

#endif /*FP_TASK_H*/
