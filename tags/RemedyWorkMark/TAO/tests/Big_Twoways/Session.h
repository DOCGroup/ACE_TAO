// -*- C++ -*-
//
// $Id$

#ifndef BIG_TWOWAYS_SESSION_H
#define BIG_TWOWAYS_SESSION_H

#include /**/ "ace/pre.h"

#include "TestS.h"
#include "Session_Task.h"

#if defined(ACE_HAS_THREADS)
# include "ace/Barrier.h"
#else
# include "ace/Null_Barrier.h"
#endif  /* ACE_HAS_THREADS */

/// Implement the Test::Session interface
class Session
  : public virtual POA_Test::Session
{
public:
  /// Constructor
  Session (Test::Session_Control_ptr control,
           CORBA::ULong payload_size,
           CORBA::ULong thread_count,
           CORBA::ULong message_count,
           CORBA::ULong peer_count);

  /// Run one of the experiment threads
  int svc (void);

  // = The skeleton methods
  virtual void start (const Test::Session_List &other_sessions);

  virtual Test::Payload* echo_payload (const Test::Payload &the_payload);

  virtual void destroy (void);

protected:
  /// Protected destructor, call _remove_ref() to delete
  virtual ~Session (void);

private:
  /// Helper function used to report any problems and destroy local
  /// resources
  void terminate (CORBA::Boolean success);

  /// Return 1 if all the work in this session has been completed
  int more_work (void) const;

  /// Validate all the connections
  void validate_connections (void);

private:
  /// Synchronize the internal state
  TAO_SYNCH_MUTEX mutex_;

  /// Keep a reference to the Session_Control, this is used to report
  /// when the test finishes.
  Test::Session_Control_var control_;

  /// Keep track of wether the test is running.
  int running_;

  /// The other session objects participating in the test
  Test::Session_List other_sessions_;

  /// Size of each message
  CORBA::ULong payload_size_;

  /// Number of threads
  CORBA::ULong thread_count_;

  /// Number of messages to send
  CORBA::ULong message_count_;

  /// The number of threads currently running, when this reaches 0 the
  /// session destroys itself.
  CORBA::ULong active_thread_count_;

  /// Number of messages expected
  CORBA::ULong expected_messages_;

  /// Helper class to run svc() in a separate thread
  Session_Task task_;

  /// Barrier to start all threads simultaenously
#if defined(ACE_HAS_THREADS)
  typedef ACE_Thread_Barrier Barrier;
#else
  typedef ACE_Null_Barrier Barrier;
#endif /* ACE_HAS_THREADS */
  Barrier barrier_;
};

#include /**/ "ace/post.h"

#endif /* BIG_TWOWAYS_SESSION_H */
