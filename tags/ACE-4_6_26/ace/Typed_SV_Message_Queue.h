/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Typed_SV_Message_Queue.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TYPED_MESSAGE_QUEUE_H
#define ACE_TYPED_MESSAGE_QUEUE_H

#include "ace/SV_Message_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Typed_SV_Message.h"

template <class T>
class ACE_Typed_SV_Message_Queue
{
  // = TITLE
  //    Defines the header file for the C++ wrapper for message queues.
public:
  enum
  {
    ACE_CREATE = IPC_CREAT,
    ACE_OPEN = 0,
    ACE_NOWAIT = IPC_NOWAIT
  };

  // = Initialization and termination operations.
  ACE_Typed_SV_Message_Queue (void);
  ACE_Typed_SV_Message_Queue (key_t external_id,
                              int create = ACE_OPEN,
                              int perms = ACE_DEFAULT_FILE_PERMS);
  int open (key_t external_id,
            int create = ACE_OPEN,
            int perms = ACE_DEFAULT_FILE_PERMS);
  int close (void);
  int remove (void);
  ~ACE_Typed_SV_Message_Queue (void);

  // = Send and recv methods.
  int send (const ACE_Typed_SV_Message<T> &mb, int mflags = 0);
  int recv (ACE_Typed_SV_Message<T> &mb, int mflags = 0);

  int control (int option, void *arg = 0);
  // Control the underlying message queue.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_SV_Message_Queue message_queue_;
};

#if defined (__ACE_INLINE__)
#include "ace/Typed_SV_Message_Queue.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Typed_SV_Message_Queue.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Typed_SV_Message_Queue.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TYPED_MESSAGE_QUEUE_H */
