// file      : ace/RMCast/Socket.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

#include "Socket.h"

namespace ACE_RMCast
{
  Socket::
  Socket (Address const& a, bool loop)
      : loop_ (loop), sn_ (1), cond_ (mutex_)
  {
    acknowledge_ = auto_ptr<Acknowledge> (new Acknowledge ());
    retransmit_ = auto_ptr<Retransmit> (new Retransmit ());
    simulator_ = auto_ptr<Simulator> (new Simulator ());
    link_ = auto_ptr<Link> (new Link (a));

    // Start IN stack from top to bottom.
    //
    in_start (0);
    acknowledge_->in_start (this);
    retransmit_->in_start (acknowledge_.get ());
    simulator_->in_start (retransmit_.get ());
    link_->in_start (simulator_.get ());

    // Start OUT stack from bottom up.
    //
    link_->out_start (0);
    simulator_->out_start (link_.get ());
    retransmit_->out_start (simulator_.get ());
    acknowledge_->out_start (retransmit_.get ());
    out_start (acknowledge_.get ());
  }

  Socket::
  ~Socket ()
  {
    // Stop OUT stack from top to bottom.
    //
    out_stop ();
    acknowledge_->out_stop ();
    retransmit_->out_stop ();
    simulator_->out_stop ();
    link_->out_stop ();

    // Stop IN stack from bottom up.
    //
    link_->in_stop ();
    simulator_->in_stop ();
    retransmit_->in_stop ();
    acknowledge_->in_stop ();
    in_stop ();
  }


  void Socket::
  send (void const* buf, size_t s)
  {
    Message_ptr m (new Message);

    m->add (Profile_ptr (new SN (sn_++)));
    m->add (Profile_ptr (new Data (buf, s)));

    // Qualification is for VC6.
    //
    Element::send (m);
  }

  size_t Socket::
  recv (void* buf, size_t s)
  {
    Lock l (mutex_);

    while (queue_.is_empty ()) cond_.wait ();

    Message_ptr m;
    if (queue_.dequeue_head (m) == -1) abort ();

    Data const* d = static_cast<Data const*>(m->find (Data::id));

    size_t r (d->size () < s ? d->size () : s);

    ACE_OS::memcpy (buf, d->buf (), r);

    return r;
  }

  void Socket::
  recv (Message_ptr m)
  {
    if (m->find (Data::id) != 0)
    {
      if (!loop_)
      {
        Address to (static_cast<To const*> (m->find (To::id))->address ());

        Address from (
          static_cast<From const*> (m->find (From::id))->address ());

        if (to == from) return;
      }

      Lock l (mutex_);

      bool signal (queue_.is_empty ());

      queue_.enqueue_tail (m);

      if (signal) cond_.signal ();
    }
  }
}
