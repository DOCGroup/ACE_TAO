// $Id$

// Task.h
//
// Tutorial regarding a way to use ACE_Stream.
//
// written by bob mcwhirter (bob@netwrench.com)

#ifndef TASK_H
#define TASK_H

#include <ace/Task.h>
#include <ace/Synch.h>

// Always typedef when possible.

typedef ACE_Task<ACE_MT_SYNCH> Task_Base;

class Task : public Task_Base
{
public:
  typedef Task_Base inherited;
  // This is just good form.

  Task (const char *nameOfTask,
        int numberOfThreads);
  // Initialize our Task with a name, and number of threads to spawn.

  virtual ~Task (void);

  virtual int open (void *arg);
  // This is provided to prevent compiler complaints about hidden
  // virtual functions.

  virtual int close (u_long flags);
  // This closes down the Task and all service threads.

  virtual int put (ACE_Message_Block *message,
                   ACE_Time_Value *timeout);
  // This is the interface that ACE_Stream uses to communicate with
  // our Task.

  virtual int svc (void);
  // This is the actual service loop each of the service threads
  // iterates through.

  const char *nameOfTask (void) const;
  // Returns the name of this Task.

private:
  int d_numberOfThreads;
  char d_nameOfTask[64];

  ACE_Barrier d_barrier;
  // Simple Barrier to make sure all of our service threads have
  // entered their loop before accepting any messages.
};

#endif /* TASK_H */
