//$Id$
#ifndef DT_CREATOR_H
#define DT_CREATOR_H

#include "FP_Scheduler.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "dt_creator_export.h"

typedef Thread_Task **DT_LIST;

class DT_Creator_Export DT_Creator : public ACE_Service_Object
{
 public:

  DT_Creator (void);

  int init (int argc, char *argv []);
  
  void create_distributable_threads (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

 private:
  DT_LIST dt_list_;
  Fixed_Priority_Scheduler* scheduler_;
  ACE_Barrier* barrier_;
  long flags_;
  int dt_count_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (DT_Creator, DT_Creator)
ACE_FACTORY_DECLARE (DT_Creator, DT_Creator)

#endif /*DT_CREATOR_H*/
