// Typed_SV_Message_Queue.cpp
// $Id$

#ifndef ACE_TYPED_SV_MESSAGE_QUEUE_C
#define ACE_TYPED_SV_MESSAGE_QUEUE_C

#include "ace/Streams/Typed_SV_Message.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Streams/Typed_SV_Message_Queue.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if !defined (__ACE_INLINE__)
#include "ace/Streams/Typed_SV_Message_Queue.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Typed_SV_Message_Queue, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Typed_SV_Message_Queue)

template <class T> void
ACE_Typed_SV_Message_Queue<T>::dump (void) const
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::dump");
}

template <class T>
ACE_Typed_SV_Message_Queue<T>::ACE_Typed_SV_Message_Queue (void)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::ACE_Typed_SV_Message_Queue");
}

template <class T>
ACE_Typed_SV_Message_Queue<T>::ACE_Typed_SV_Message_Queue (key_t external_id,
                                                           int create,
                                                           int perms)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::ACE_Typed_SV_Message_Queue");
  if (this->open (external_id, create, perms) == -1)
    ACE_ERROR ((LM_ERROR,
                "ACE_Typed_SV_Message_Queue::ACE_Typed_SV_Message_Queue"));
}

template <class T>
ACE_Typed_SV_Message_Queue<T>::~ACE_Typed_SV_Message_Queue (void)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::~ACE_Typed_SV_Message_Queue");
}

#endif /* ACE_TYPED_SV_MESSAGE_QUEUE_C */
