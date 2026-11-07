// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#ifndef TMCAST_MESSAGING_HPP
#define TMCAST_MESSAGING_HPP

#include <ace/Synch.h>
#include <ace/Bound_Ptr.h>

#include "MTQueue.hpp"

namespace ACE_TMCast
{
  class Message
  {
  public:
    virtual ~Message () = default;
  };

  using MessagePtr = ACE_Strong_Bound_Ptr<Message, ACE_SYNCH_MUTEX>;

  using MessageQueue =
    MTQueue<MessagePtr, ACE_Thread_Mutex, ACE_Condition<ACE_Thread_Mutex>>;

  struct MessageQueueAutoLock
  {
    explicit MessageQueueAutoLock (MessageQueue& q)
        : q_ (q)
    {
      q_.lock ();
    }

    MessageQueueAutoLock (MessageQueueAutoLock const&) = delete;
    MessageQueueAutoLock (MessageQueueAutoLock&&) = delete;
    MessageQueueAutoLock& operator= (MessageQueueAutoLock const&) = delete;
    MessageQueueAutoLock& operator= (MessageQueueAutoLock&&) = delete;

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
