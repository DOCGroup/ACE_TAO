// Typed_SV_Message_Queue.cpp
// $Id$

#if !defined (ACE_TYPED_SV_MESSAGE_QUEUE_C)
#define ACE_TYPED_SV_MESSAGE_QUEUE_C

#define ACE_BUILD_DLL
#include "ace/Typed_SV_Message.h"
#include "ace/Typed_SV_Message_Queue.h"

#if !defined (__ACE_INLINE__)
#include "ace/Typed_SV_Message_Queue.i"
#endif /* __ACE_INLINE__ */

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
