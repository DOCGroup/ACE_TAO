// $Id$

#include "tao/Reply_Dispatcher.h"
#include "ace/Synch_T.h"

ACE_RCSID(tao, Reply_Dispatcher, "$Id$")

#if !defined (__ACE_INLINE__)
#include "tao/Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

// Constructor.
TAO_Reply_Dispatcher::TAO_Reply_Dispatcher (void)
  // Just an invalid reply status.
  : reply_status_ (100),
    mutex_ (),
    condition_ (this->mutex_),
    timeout_ (TAO_Reply_Dispatcher::NO_TIMEOUT),
    dispatching_ (0),
    threads_waiting_ (0)
{
}

// Destructor.
TAO_Reply_Dispatcher::~TAO_Reply_Dispatcher (void)
{
}

void
TAO_Reply_Dispatcher::start_dispatch (void)
{
  if (this->timeout_ != TAO_Reply_Dispatcher::TIMEOUT)
    return;

  {
    ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX,
                       guard,
                       this->mutex_));

    this->dispatching_ = 1;
  }
}


void
TAO_Reply_Dispatcher::end_dispatch (void)
{
  if (this->timeout_ != TAO_Reply_Dispatcher::TIMEOUT)
    return;
  {
    ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX,
                       ace_mon,
                       this->mutex_));

    this->dispatching_ = 0;

    if (this->threads_waiting_)
      this->condition_.signal ();
  }
}

int
TAO_Reply_Dispatcher::wait_for_dispatch_completion (void)
{
  if (this->timeout_ != TAO_Reply_Dispatcher::TIMEOUT)
    return -1;

  if (this->dispatching_)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                ace_mon,
                                this->mutex_,
                                -1));

      // The dispatching could have ended by now, in which case just
      // return.
      if (this->dispatching_ == 0)
        return 0;

      // Mark the number of waiting threads
      ++this->threads_waiting_;

      this->condition_.wait ();

      --this->threads_waiting_;
      return 0;
    }

  return -1;
}
