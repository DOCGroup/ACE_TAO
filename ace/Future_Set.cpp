// Future.cpp
// $Id$

#define ACE_BUILD_DLL

#ifndef ACE_FUTURE_SET_CPP
#define ACE_FUTURE_SET_CPP

#include "ace/Future_Set.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Future_Set, "$Id$")

#if defined (ACE_HAS_THREADS)

template <class T>
ACE_Future_Set<T>::ACE_Future_Set(ACE_Message_Queue<ACE_SYNCH> *new_queue)
  : delete_queue_ (0)
{
  if (new_queue)
    this->future_notification_queue_ = new_queue;
  else
    {
      ACE_NEW (this->future_notification_queue_,
               ACE_Message_Queue<ACE_SYNCH>);
      this->delete_queue_ = 1;
    }
}

template <class T>
ACE_Future_Set<T>::ACE_Future_Set(const ACE_Future_Set<T> &r)
{
}

template <class T>
ACE_Future_Set<T>::~ACE_Future_Set(void)
{
  // Detach ourselves from all remaining futures, if any,
  // in our list.
  for (FUTURE_NODE *node = this->future_list_.delete_head ();
       node != 0;
       node = this->future_list_.delete_head ())
    {
      node->item_.detach (this);
      delete node;
    }

  if (this->delete_queue_ != 0)
    delete this->future_notification_queue_;
}

template <class T> int
ACE_Future_Set<T>::is_empty() const
{
  return this->future_list_.is_empty ();
}

template <class T> void
ACE_Future_Set<T>::insert (ACE_Future<T> &future)
{
  FUTURE_NODE *node;
  ACE_NEW (node,
           FUTURE_NODE (future));
  this->future_list_.insert_tail (node);

  // Attach ourself to the ACE_Futures list of observer
  future.attach (this);
}

template <class T> void
ACE_Future_Set<T>::update (const ACE_Future<T> &future)
{
  ACE_Message_Block *mb;
  ACE_NEW (mb,
           ACE_Message_Block ((char *) 0, 0));

  // Enqueue in priority order.
  this->future_notification_queue_->enqueue (mb, 0);
}

template <class T> int
ACE_Future_Set<T>::next_readable (ACE_Future<T> &future,
                                  ACE_Time_Value *tv)
{
  if (this->is_empty ())
    return 0;

  ACE_Message_Block *mb;

  // Wait for a "readable future" signal from the message queue.
  if (this->future_notification_queue_->dequeue_head (mb,
                                                      tv) != -1)
    // Delete the message block.
    mb->release ();
  else
    return 0;

  // Remove all nodes containing the specified future from our list.
  int count = 0;
  FUTURE_NODE *node = 0;

  for (FUTURE_LIST::ITERATOR iter (this->future_list_);
       (node = iter.next ()) != 0;
       iter.advance ())
    {
      ++count;
      if (node->item_.ready ())
        {
          future = node->item_;
          this->future_list_.remove (node);
          delete node;

          // NOTE: if the user inserted the same future into the list
          // more than once, then maybe I should loop through the
          // remaining futures in the list and remove all of those
          // futures which are equal to the one we are returning.
          return 1;
        }
    }

  return 0;
}

#endif /* ACE_HAS_THREADS */
#endif /* ACE_FUTURE_SET_CPP */

