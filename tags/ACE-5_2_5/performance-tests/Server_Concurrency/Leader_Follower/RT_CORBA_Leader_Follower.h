//=============================================================================
/**
 *  @file    RT_CORBA_Leader_Follower.h
 *
 *  $Id$
 *
 *   The leader follower test modified to suit RTCORBA tests. The
 *   original  leader follower test has too many things that doesnt
 *   get what we are looking for (read it as just confusing enough)
 *   . Hence a new test.
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef PERF_TEST_RT_CORBA_LEADER_FOLLOWER_H
#define PERF_TEST_RT_CORBA_LEADER_FOLLOWER_H
#include "ace/pre.h"
#include "ace/Task.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

#include "ace/Synch_T.h"

enum DEBUGGING_RANGE
{
  DEBUG_NONE = 0,
  DEBUG
};


/**
 * @class Synchronisers
 *
 * @brief This class provides all the synchrnoisers used in this
 *  test.
 *
 *
 */

/// Forward declaration..
class Leader_Follower_Task;

class Synchronisers
{
public:
  friend class Leader_Follower_Task;

  /// Ctor
  Synchronisers (void);

  int  start_synchronization (void);
  int  end_synchronization (void);

private:

  /// The mutex and condition variable that is used for
  /// synchronisation.
  ACE_SYNCH_MUTEX mutex_;

  ACE_SYNCH_CONDITION condition_;

  /// A manual event
  ACE_Manual_Event event_;
};


/**
 * @class Leader_Follower_Task
 *
 * @brief This class provide the interface and an implementation to
 *  simulate how the LF pattern in the ORB would behave.
 *
 *
 *  Basically all threads ready to do an upcall would wait on a token
 *  or lock. The thread that gets the token before it gets ready to do
 *  the upcall would wake up a thread waiting on the token.
 *
 */

class Leader_Follower_Task : public ACE_Task_Base
{
public:

  /// Ctor with a mutex and a condition variable
  Leader_Follower_Task (Synchronisers &synch);

  /// Messages consumed
  int processed (void);

  /// Method that is run by a daemon thread
  int svc (void);

private:
  /// Number of messages processed by this thread
  int messages_consumed_;

  /// Our reference to the synchroniser
  Synchronisers &synch_;
};

#endif /*ACE_HAS_THREADS*/

#include "ace/post.h"
#endif /*PERF_TEST_RT_CORBA_LEADER_FOLLOWER*/
