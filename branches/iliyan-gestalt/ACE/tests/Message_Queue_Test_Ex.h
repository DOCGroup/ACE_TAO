// -*- C++ -*-

//=============================================================================
/**
 * @file Message_Queue_Test_Ex.h
 *
 * $Id$
 *
 * Define class needed for generating templates. IBM C++ requires this
 * to be in its own file for auto template instantiation.
 *
 * @author Michael Vitlo <mvitalo@sprynet.com>
 * @author Irfan Pyarali <irfan@cs.wustl.edu>
 * @author David L. Levine <levine@cs.wustl.edu>
 * @author Guy Peleg <guy.peleg@amdocs.com>
 */
//=============================================================================

#ifndef ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H
#define ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H

#include "ace/OS_NS_string.h"

// User-defined class used for queue data.
class User_Class
{
public:
  User_Class (const char inputMsg[])
    : message_ (0),
      next_(0)
  {
    ACE_NEW (this->message_, char[ACE_OS::strlen (inputMsg) + 1]);
    ACE_OS::strcpy (this->message_, inputMsg);
  }

  ~User_Class (void) { delete [] this->message_; }

  const char *message (void) const
  {
    return this->message_;
  }

  // This is for checking the ACE_Message_Queue_Ex_N
  User_Class *next () const
  {
    return this->next_;
  }

  void next (User_Class *uc)
  {
    this->next_ = uc;
  }

private:
  char *message_;
  User_Class *next_;
};

// The main tests for the ACE_Message_Queue_Ex_N
struct Receive_Messages;

class MQ_Ex_N_Tester
{
public:
  int single_thread_performance_test (void);

#if defined (ACE_HAS_THREADS)
  int performance_test (void);

  /// Sender runs with an autonomous thread
  static ACE_THR_FUNC_RETURN sender (void *);

  /// Receiver runs with an autonomous thread
  static ACE_THR_FUNC_RETURN receiver (void *);

  /// Multi threaded tests use this queue
  ACE_Message_Queue_Ex_N<User_Class, ACE_MT_SYNCH>   mt_queue_;
#endif /* ACE_HAS_THREADS */

  /// Single threaded tests use this queue
  ACE_Message_Queue_Ex_N<User_Class, ACE_NULL_SYNCH> st_queue_;

private:
  /// Helper methods
  int test_enqueue_head (void);
  int test_enqueue_tail (void);
};

#endif /* ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H */
