/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Task_T.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TASK_T_H
#define ACE_TASK_T_H

#include "ace/Message_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"
#include "ace/Task.h"

// Forward decls...
template <ACE_SYNCH_DECL> class ACE_Module;

template <ACE_SYNCH_DECL>
class ACE_Task : public ACE_Task_Base
{
  // = TITLE
  //    Primary interface for application message processing, as well
  //    as input and output message queueing.
  //
  // = DESCRIPTION
  //    This class serves as the basis for passive and active objects
  //    in ACE.
public:
  friend class ACE_Module<ACE_SYNCH_USE>;
  friend class ACE_Module_Type;

  // = Initialization/termination methods.
  ACE_Task (ACE_Thread_Manager *thr_mgr = 0,
            ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0);
  // Initialize a Task, supplying a thread manager and a message
  // queue.  If the user doesn't supply a ACE_Message_Queue pointer
  // then we'll allocate one dynamically.  Otherwise, we'll use the
  // one they give.

  virtual ~ACE_Task (void);
  // Destructor.

  ACE_Message_Queue<ACE_SYNCH_USE> *msg_queue (void);
  // Gets the message queue associated with this task.

  void msg_queue (ACE_Message_Queue<ACE_SYNCH_USE> *);
  // Sets the message queue associated with this task.

public: // Should be protected:
  // = Message queue manipulation methods.

  // = Enqueue and dequeue methods.

  // For the following five method if <timeout> == 0, the caller will
  // block until action is possible, else will wait until the
  // <{absolute}> time specified in *<timeout> elapses).  These calls
  // will return, however, when queue is closed, deactivated, when a
  // signal occurs, or if the time specified in timeout elapses, (in
  // which case errno = EWOULDBLOCK).

  int putq (ACE_Message_Block *, ACE_Time_Value *timeout = 0);
  // Insert message into the message queue.  Note that <timeout> uses
  // <{absolute}> time rather than <{relative}> time.

  int getq (ACE_Message_Block *&mb, ACE_Time_Value *timeout = 0);
  // Extract the first message from the queue (blocking).  Note that
  // <timeout> uses <{absolute}> time rather than <{relative}> time.

  int ungetq (ACE_Message_Block *, ACE_Time_Value *timeout = 0);
  // Return a message to the queue.  Note that <timeout> uses
  // <{absolute}> time rather than <{relative}> time.

  int reply (ACE_Message_Block *, ACE_Time_Value *timeout = 0);
  // Turn the message around and send it back down the Stream.  Note
  // that <timeout> uses <{absolute}> time rather than <{relative}>
  // time.

  int put_next (ACE_Message_Block *msg, ACE_Time_Value *timeout = 0);
  // Transfer message to the adjacent ACE_Task in a ACE_Stream.  Note
  // that <timeout> uses <{absolute}> time rather than <{relative}>
  // time.

  int can_put (ACE_Message_Block *);
  // Tests whether we can enqueue a message without blocking.

  // = ACE_Task utility routines to identify names et al.
  const ASYS_TCHAR *name (void) const;
  // Return the name of the enclosing Module if there's one associated
  // with the Task, else returns 0.

  // = Pointers to next ACE_Task_Base (if ACE is part of an ACE_Stream).
  ACE_Task<ACE_SYNCH_USE> *next (void);
  // Get next Task pointer.
  void next (ACE_Task<ACE_SYNCH_USE> *);
  // Set next Task pointer.

  ACE_Task<ACE_SYNCH_USE> *sibling (void);
  // Return the Task's sibling if there's one associated with the
  // Task's Module, else returns 0.

  ACE_Module<ACE_SYNCH_USE> *module (void) const;
  // Return the Task's Module if there is one, else returns 0.

  int flush (u_long flag = ACE_Task_Flags::ACE_FLUSHALL);
  // Flush the queue.  Note that if this conflicts with the C++
  // iostream flush() function, just rewrite the iostream function as
  // ::flush().

  // = Special routines corresponding to certain message types.

  void water_marks (ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds, size_t);
  // Manipulate watermarks.

  ACE_Message_Queue<ACE_SYNCH_USE> *msg_queue_;
  // Queue of messages on the ACE_Task..

  int delete_msg_queue_;
  // 1 if should delete Message_Queue, 0 otherwise.

  ACE_Module<ACE_SYNCH_USE> *mod_;
  // Back-pointer to the enclosing module.

  ACE_Task<ACE_SYNCH_USE> *next_;
  // Pointer to adjacent ACE_Task.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Task<ACE_SYNCH_USE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Task (const ACE_Task<ACE_SYNCH_USE> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Task_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Task_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Task_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TASK_T_H */
