/* -*- c++ -*- */
#ifndef JAWS_CONCURRENCY_H
#define JAWS_CONCURRENCY_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Synch_Traits.h"
#include <memory>

#include "JAWS/Export.h"
#include "JAWS/Jaws_IO.h"

class JAWS_Data_Block;
class JAWS_Dispatch_Policy;
class JAWS_Reaper;

/**
 * Base class for different concurrency models.
 *
 * Provides a default put() implementation that calls putq(). Derived classes
 * can override it to process work synchronously, for example by calling
 * put_next().
 */
class JAWS_Export JAWS_Concurrency_Base : public ACE_Task<ACE_SYNCH>
{
public:
  JAWS_Concurrency_Base ();
  ~JAWS_Concurrency_Base () override;
  JAWS_Concurrency_Base (JAWS_Concurrency_Base const &) = delete;
  JAWS_Concurrency_Base (JAWS_Concurrency_Base &&) = delete;
  JAWS_Concurrency_Base &operator= (JAWS_Concurrency_Base const &) = delete;
  JAWS_Concurrency_Base &operator= (JAWS_Concurrency_Base &&) = delete;

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;
  int svc () override;

  /// Run the service loop; thread-pool workers loop indefinitely, while
  /// thread-per-request workers process one request.
  virtual int svc_loop (JAWS_Data_Block *db);

  /// Follow the pipeline tasks to process the data block.
  virtual int svc_hook (JAWS_Data_Block *db);

  /// Handle the callback made when an accept completes.
  virtual int activate_hook ();

  virtual ACE_Message_Block *singleton_mb ();

protected:
  int mb_acquired_;
  ACE_Message_Block *mb_;
  std::unique_ptr<JAWS_Reaper> reaper_;
  ACE_SYNCH_MUTEX lock_;
};

/**
 * Routes completed I/O to the appropriate concurrency mechanism.
 * JAWS_IO_Handler uses the dispatcher to find a thread for completed I/O.
 */
class JAWS_Export JAWS_Dispatcher
{
public:
  JAWS_Dispatcher ();

  int dispatch (ACE_Message_Block *mb);
  JAWS_Dispatch_Policy *policy ();
  JAWS_Dispatch_Policy *policy (JAWS_Dispatch_Policy *p);

private:
  JAWS_Dispatch_Policy *policy_;
};

/// Runs requests using a pool of worker threads.
class JAWS_Export JAWS_Thread_Pool_Task : public JAWS_Concurrency_Base
{
public:
  /// Start the thread-pool task.
  virtual int make (long flags, int nthreads, int maxthreads);

private:
  long flags_;
  int nthreads_;
  int maxthreads_;
};

/// Runs each request on a separate worker thread.
class JAWS_Export JAWS_Thread_Per_Task : public JAWS_Concurrency_Base
{
public:
  /// Start the thread-per-request task.
  virtual int make (long flags, int maxthreads);

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;

  int svc_loop (JAWS_Data_Block *db) override;

  int activate_hook () override;

private:
  long flags_;
  int maxthreads_;
};

using JAWS_Dispatcher_Singleton = ACE_Singleton<JAWS_Dispatcher, ACE_SYNCH_MUTEX>;

using JAWS_Thread_Pool_Singleton = ACE_Singleton<JAWS_Thread_Pool_Task, ACE_SYNCH_MUTEX>;

using JAWS_Thread_Per_Singleton = ACE_Singleton<JAWS_Thread_Per_Task, ACE_SYNCH_MUTEX>;

#endif /* !defined (JAWS_CONCURRENCY_H) */
