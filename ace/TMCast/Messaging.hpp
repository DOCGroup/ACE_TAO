// file      : TMCast/Messaging.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TMCAST_MESSAGING_HPP
#define TMCAST_MESSAGING_HPP

#include <ace/Synch.h>
#include <ace/Refcounted_Auto_Ptr.h>

#include "MTQueue.hpp"

namespace TMCast
{
  class Message
  {
  public:
    virtual
    ~Message () {}
  };

  typedef
  ACE_Refcounted_Auto_Ptr<Message, ACE_Null_Mutex>
  MessagePtr;

  typedef
  MTQueue<MessagePtr, ACE_Thread_Mutex, ACE_Condition<ACE_Thread_Mutex> >
  MessageQueue;

  struct MessageQueueAutoLock
  {
    MessageQueueAutoLock (MessageQueue& q)
        : q_ (q)
    {
      q_.lock ();
    }

    void
    unlock ()
    {
      q_.unlock ();
    }

    ~MessageQueueAutoLock ()
    {
    q_.unlock ();
    }

  private:
    MessageQueue& q_;
  };
}

#endif // TMCAST_MESSAGING_HPP
