
// $Id$

#ifndef TASK_H
#define TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Task : public ACE_Task < ACE_MT_SYNCH >
{
public:

  typedef ACE_Task < ACE_MT_SYNCH > inherited;

    Task (void);
   ~Task (void);

  int start (int threads = 1);

  int svc (void);

  int close (u_long flags = 0);

protected:
    ACE_Barrier * barrier_;
};

#endif
