//$Id$

#ifndef FP_DT_CREATOR_H
#define FP_DT_CREATOR_H

#include "../DT_Creator.h"
#include "fp_dt_creator_export.h"
#include "../Task.h"

class FP_DT_Creator_Export FP_DT_Creator: public DT_Creator
{
public:
  FP_DT_Creator (void);

  virtual CORBA::Policy_ptr sched_param (int importance);

  //virtual Task* task (void);
  virtual Thread_Task* create_thr_task (int importance,
					int start_time,
					int load);
  
  virtual void yield (int suspend_time);
};


ACE_STATIC_SVC_DECLARE_EXPORT (FP_DT_Creator, FP_DT_Creator)
ACE_FACTORY_DECLARE (FP_DT_Creator, FP_DT_Creator)

#endif /*FP_DT_CREATOR_H*/
