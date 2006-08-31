#ifndef _SYSTEM_MONITOR_TASK_H_
#define _SYSTEM_MONITOR_TASK_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "System_Monitor_exec_export.h"
#include "ace/Task.h"

namespace CUTS
{
  namespace CIDL_System_Monitor_Impl
  {
    class SYSTEM_MONITOR_EXEC_Export System_Monitor_Task :
      public ACE_Task_Base
    {
    public:
      System_Monitor_Task (bool & active);

      virtual ~System_Monitor_Task (void);

    private:
      /// Service handler for the task.
      int svc (void);

      bool & active_;
    };
  }
}

#include /**/ "ace/post.h"

#endif  // !defined _SYSTEM_MONITOR_TASK_H_
