/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_CONCURRENCY_H)
#define JAWS_CONCURRENCY_H

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Task.h"

#include "JAWS/IO.h"

class JAWS_Dispatch_Policy;

class JAWS_Concurrency_Base : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //     Base class for different concurrency models
  //
  // = DESCRIPTION
  //     Provides a default implementaion of the virtual put() method
  //     which calls putq(), but can be overloaded to do something
  //     synchronously, such as call put_next().

{
public:
  JAWS_Concurrency_Base (void);

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
  virtual int svc (void);
};

class JAWS_Dispatcher
  // = TITLE
  //     The class that is responsible to delivering events to the
  //     appropriate concurrency mechanism.
  //
  // = DESCRIPTION
  //     JAWS_IO_Handler calls into the dispatcher so that the completed
  //     IO can find a thread to take care of it.
{
public:
  JAWS_Dispatcher (void);

  int dispatch (ACE_Message_Block *mb);
  JAWS_Dispatch_Policy *policy (void);
  JAWS_Dispatch_Policy *policy (JAWS_Dispatch_Policy *p);

private:
  JAWS_Dispatch_Policy *policy_;
};

class JAWS_Thread_Pool_Task : public JAWS_Concurrency_Base
  // = TITLE
  //     Used to implement Thread Pool Concurrency Strategy
  //
  // = DESCRIPTION
  //     This task is created to hold a pool of threads that receive
  //     requests through the message queue.
{
public:
  virtual int open (long flags = THR_NEW_LWP,
                    int nthreads = 5, int maxthreads = 20);
  // Initiate the thread_pool task

private:
  int nthreads_;
  int maxthreads_;
};

class JAWS_Thread_Per_Task : public JAWS_Concurrency_Base
  // = TITLE
  //     Used to implement Thread Per Request Concurrency Strategy
  //
  // = DESCRIPTION
  //     As each new message arrives from the queue, a new thread is
  //     spawned to handle it.  This is done by overloading put to call
  //     activate.
{
public:
  JAWS_Thread_Per_Task (long flags = THR_NEW_LWP, int maxthreads = 20);

  virtual int open (long flags = THR_NEW_LWP, int maxthreads = 20);
  // Initiate the thread_per task

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);

private:
  long flags_;
  int maxthreads_;
};

typedef ACE_Singleton<JAWS_Dispatcher, ACE_SYNCH_MUTEX>
        JAWS_Dispatcher_Singleton;

typedef ACE_Singleton<JAWS_Thread_Pool_Task, ACE_SYNCH_MUTEX>
        JAWS_Thread_Pool_Singleton;

typedef ACE_Singleton<JAWS_Thread_Per_Task, ACE_SYNCH_MUTEX>
        JAWS_Thread_Per_Singleton;

#endif /* !defined (JAWS_CONCURRENCY_H) */
