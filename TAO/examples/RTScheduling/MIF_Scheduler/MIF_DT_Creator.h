//$Id$

#ifndef MIF_DT_CREATOR_H
#define MIF_DT_CREATOR_H

#include "../DT_Creator.h"
#include "mif_dt_creator_export.h"

class MIF_DT_Creator_Export MIF_DT_Creator: public DT_Creator
{
public:
  MIF_DT_Creator (void);

  virtual CORBA::Policy_ptr sched_param (int importance);

  //virtual Task* task (void);
  virtual Thread_Task* create_thr_task (int importance,
					int start_time,
					int load,
					int iter,
					int dist,
					char *job_name);
  
  virtual void yield (int suspend_time,
		      Thread_Task* task);

  virtual void wait (void);
  virtual int total_load (void);
};


ACE_STATIC_SVC_DECLARE_EXPORT (MIF_DT_Creator, MIF_DT_Creator)
ACE_FACTORY_DECLARE (MIF_DT_Creator, MIF_DT_Creator)

#endif /*MIF_DT_CREATOR_H*/

