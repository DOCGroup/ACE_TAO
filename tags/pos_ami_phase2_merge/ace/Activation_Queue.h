/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Activation_Queue.h
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_ACTIVATION_QUEUE_H
#define ACE_ACTIVATION_QUEUE_H

#include "ace/Synch_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Queue.h"
#include "ace/Method_Request.h"

class ACE_Export ACE_Activation_Queue
{
  // = TITLE
  //     Reifies a method into a request.  Subclasses typically
  //     represent necessary state and behavior.
  //
  // = DESCRIPTION
  //     A <Method_Request> is inserted in the <Activation_Queue>,
  //     where it is subsequently removed by the <Scheduler>, which
  //     invokes its <call> method..
public:
  // = Initialization and termination methods.
  ACE_Activation_Queue (ACE_Message_Queue<ACE_SYNCH> *new_queue = 0);
  // Constructor.

  virtual ~ACE_Activation_Queue (void);
  // Destructor.

  // = Activate Queue operations.

  // For the following two methods if <timeout> == 0, the caller will
  // block until action is possible, else will wait until the absolute
  // time specified in *<timeout> elapses.  These calls will return,
  // however, when queue is closed, deactivated, when a signal occurs,
  // or if the time specified in timeout elapses, (in which case errno
  // = EWOULDBLOCK).

  ACE_Method_Request *dequeue (ACE_Time_Value *tv = 0);
  // Dequeue the next available <Method_Request>.

  int enqueue (ACE_Method_Request *new_method_request,
               ACE_Time_Value *tv = 0);
  // Enqueue the <Method_Request> in priority order.  The priority is
  // determined by the <priority> method of the <new_message_request>.

  void dump (void) const;
  // Dump the state of an request.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Message_Queue<ACE_SYNCH> *queue_;
  // Stores the <Method_Requests>.

  int delete_queue_;
  // Keeps track of whether we need to delete the queue.
};

#if defined (__ACE_INLINE__)
#include "ace/Activation_Queue.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ACTIVATION_QUEUE_H */

