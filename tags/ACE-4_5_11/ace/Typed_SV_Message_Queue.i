/* -*- C++ -*- */
// $Id$

// Typed_SV_Message_Queue.i

#include "ace/SV_Message_Queue.h"

template <class T> ACE_INLINE int 
ACE_Typed_SV_Message_Queue<T>::open (key_t external_id, 
				     int create, 
				     int perms)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::open");
  return this->message_queue_.open (external_id, create, perms);
}

/* What does it mean to close a message queue?! */

template <class T> ACE_INLINE int 
ACE_Typed_SV_Message_Queue<T>::close (void)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::close");
  return 1;
}

template <class T> ACE_INLINE int
ACE_Typed_SV_Message_Queue<T>::recv (ACE_Typed_SV_Message<T> &mb, 
				     int mflags)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::recv");
  int length = this->message_queue_.recv ((ACE_SV_Message &) mb, 
					  mb.max_size (), mb.type (), mflags);

  if (length != -1)
    mb.length (length);

  return length;
}

template <class T> ACE_INLINE int
ACE_Typed_SV_Message_Queue<T>::send (const ACE_Typed_SV_Message<T> &mb, 
				     int mflags)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::send");
  return this->message_queue_.send ((ACE_SV_Message &) mb, 
				    mb.length (), mflags);
}

template <class T> ACE_INLINE int
ACE_Typed_SV_Message_Queue<T>::remove (void)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::remove");
  return this->message_queue_.remove ();
}

template <class T> ACE_INLINE int
ACE_Typed_SV_Message_Queue<T>::control (int option, 
					void *arg)
{
  ACE_TRACE ("ACE_Typed_SV_Message_Queue<T>::control");
  return this->message_queue_.control (option, arg);
}
