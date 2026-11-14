/* -*- c++ -*- */
#ifndef JAWS_CONCURRENCY_H
#define JAWS_CONCURRENCY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "JAWS_IO.h"

/**
 * @brief Base class for different concurrency models.
 *
 * Provides a default implementation of put() that calls putq(). Derived
 * classes can override it to dispatch work synchronously, for example by
 * calling put_next().
 */
class JAWS_Concurrency_Base : public ACE_Task<ACE_SYNCH>
{
public:
  JAWS_Concurrency_Base () = default;
  JAWS_Concurrency_Base (JAWS_Concurrency_Base const &) = delete;
  JAWS_Concurrency_Base (JAWS_Concurrency_Base &&) = delete;
  JAWS_Concurrency_Base &operator= (JAWS_Concurrency_Base const &) = delete;
  JAWS_Concurrency_Base &operator= (JAWS_Concurrency_Base &&) = delete;
  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;
  int svc () override;
};

/**
 * @brief Selects a concurrency model based on application state.
 *
 * For now, this policy always returns the same model.
 */
class JAWS_Dispatch_Policy
{
public:
  JAWS_Dispatch_Policy () = default;
  virtual ~JAWS_Dispatch_Policy () = default;
  JAWS_Dispatch_Policy (JAWS_Dispatch_Policy const &) = delete;
  JAWS_Dispatch_Policy (JAWS_Dispatch_Policy &&) = delete;
  JAWS_Dispatch_Policy &operator= (JAWS_Dispatch_Policy const &) = delete;
  JAWS_Dispatch_Policy &operator= (JAWS_Dispatch_Policy &&) = delete;
  virtual JAWS_Concurrency_Base * update (void *state = nullptr) = 0;
};

/**
 * @brief Routes completed I/O events to the selected concurrency mechanism.
 *
 * JAWS_IO_Handler calls the dispatcher when I/O completes so a worker thread
 * can process the result.
 */
class JAWS_Dispatcher
{
public:
  explicit JAWS_Dispatcher (JAWS_Dispatch_Policy *policy);
  JAWS_Dispatcher (JAWS_Dispatcher const &) = delete;
  JAWS_Dispatcher (JAWS_Dispatcher &&) = delete;
  JAWS_Dispatcher &operator= (JAWS_Dispatcher const &) = delete;
  JAWS_Dispatcher &operator= (JAWS_Dispatcher &&) = delete;

  int dispatch (JAWS_IO_Handler *ioh);

private:
  JAWS_Dispatch_Policy *policy_;
};

/**
 * @brief Implements the thread pool concurrency strategy.
 *
 * A pool of threads receives requests through the message queue.
 */
class JAWS_Thread_Pool_Task : public JAWS_Concurrency_Base
{
public:
  JAWS_Thread_Pool_Task (long flags = THR_NEW_LWP,
                         int nthreads = 5,
                         int maxthreads = 20);

private:
  int nthreads_;
  int maxthreads_;
};

/**
 * @brief Implements the thread per request concurrency strategy.
 *
 * Each queued request starts a thread through the overridden put().
 */
class JAWS_Thread_Per_Task : public JAWS_Concurrency_Base
{
public:
  JAWS_Thread_Per_Task (long flags = THR_NEW_LWP, int maxthreads = 20);

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;

private:
  long flags_;
  int maxthreads_;
};

#endif /* !defined (JAWS_CONCURRENCY_H) */
