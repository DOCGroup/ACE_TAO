/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    RT_CORBA_Workers.h
 *
 *  $Id$
 *
 *   The Workes test modified to suit RTCORBA tests. The  original
 *   Workers has too many things that doesnt get what we are looking
 *   for (read it as just confusing enough). Hence a new test.
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef PERF_TEST_RT_CORBA_WORKERS_H
#define PERF_TEST_RT_CORBA_WORKERS_H
#include /**/ "ace/pre.h"

#include "ace/Task_T.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

enum DEBUGGING_RANGE
{
  DEBUG_NONE = 0,
  DEBUG
};

typedef ACE_Message_Queue<ACE_MT_SYNCH> Message_Queue;

/**************************************************************/
/**
 * @class Synchronisers
 *
 * @brief This class provides all the synchrnoisers used in this
 *  test.
 *
 *
 */

/// Forward declaration..
class IO_Task;
class Worker_Task;

class Synchronisers
{
public:
  friend class IO_Task;
  friend class Worker_Task;

  enum
  {
    /// Used in the message block to stop processing of message
    /// blocks.
    MB_STOP_FLAG = 0x2000
  };

  /// Ctor
  Synchronisers (void);

  int  start_synchronization (void);
  int  end_synchronization (void);

private:

  /// The mutex that is used for synchronisation.
  ACE_SYNCH_MUTEX mutex_;

  /// A manual event
  ACE_Manual_Event event_;
};


/**************************************************************/
/**
 * @class Input_Task
 *
 * @brief Class that does the Input work ie. puts the events into the
 *  message queue
 */

class Input_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /// Ctor
  Input_Task (Message_Queue *mq,
              Synchronisers &synch);

  /// The thread runs inside this method..
  int svc (void);

private:

  /// Our referance to Synchronisers
  Synchronisers &synch_;
};

/**************************************************************/
/**
 * @class Output_Task
 *
 * @brief Class that does the Output work ie. getsx the events into the
 *  message queue
 */

class Output_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /// Ctor
  Output_Task (Message_Queue *mq,
              Synchronisers &synch);

  /// The thread runs inside this method..
  int svc (void);

  /// Need to overload this method to do anything useful..
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);

private:

  /// Our referance to Synchronisers
  Synchronisers &synch_;
};


/**************************************************************/
/**
 * @class Synchronisers
 *
 * @brief This class provides and interface for all the upcall threads
 *  to run and do the actual work
 */

class Worker_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /// Ctor
  Worker_Task (Message_Queue *mq,
               Synchronisers &synch);

  /// Methods in which the threads run on
  int svc (void);

  /// Processed messages
  int processed (void);
private:

  /// Our referance to Synchronisers
  Synchronisers &synch_;

  /// Number of messages processed by us
  size_t messages_processed_;
};




#endif /*ACE_HAS_THREADS*/

#include /**/ "ace/post.h"
#endif /*PERF_TEST_RT_CORBA_WORKERS_H*/
