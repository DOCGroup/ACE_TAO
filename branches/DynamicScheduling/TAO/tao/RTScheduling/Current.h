//$Id$
#ifndef TAO_RTSCHEDULER_CURRENT_H
#define TAO_RTSCHEDULER_CURRENT_H


#include "rtscheduler_export.h"
#include "RTSchedulerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTScheduler_Current;
class TAO_TSS_Resources;

class TAO_RTScheduler_Export TAO_RTScheduler_Current: 
public RTScheduler::Current,
  public TAO_Local_RefCounted_Object
{
 public:
  TAO_Scheduler_Current (void);
  
  RTScheduler::Current_ptr get_Current (void);

  
};











#endif /*TAO_RTSCHEDULER_CURRENT_H*/
