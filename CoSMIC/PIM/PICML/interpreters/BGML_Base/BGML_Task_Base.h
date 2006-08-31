//
// $Id$
//

#ifndef BGML_TASK_BASE_H
#define BGML_TASK_BASE_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#include "BGML/BGML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class BGML_Export BGML_Task_Base : public ACE_Task_Base
{
public:

  BGML_Task_Base (int priority);
  // Constructor for Task_Base

  BGML_Task_Base ();
  // Constructor that uses FIFO priority

  virtual ~BGML_Task_Base ();
  // Destructor

  int svc (void) = 0;
  /// Thread entry point

protected:
  int set_priority ();
  // Set the Scheduling parameters for this thread

  int set_priority (int priority);
  // Change the priority to @a priority

  int priority_;
  // Priority Level of the task

};

#include /**/ "ace/post.h"

#endif /* BGML_TASK_BASE_H */
