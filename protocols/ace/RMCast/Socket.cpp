// file      : ace/RMCast/Socket.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h" // gettimeofday

#include "ace/Unbounded_Queue.h"

#include "ace/Pipe.h"

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

#include "Fragment.h"
#include "Reassemble.h"
#include "Acknowledge.h"
#include "Retransmit.h"
#include "Link.h"

#include "Socket.h"

namespace ACE_RMCast
{
  class Socket_Impl : protected Element
  {
  public:
    ~Socket_Impl ();

    Socket_Impl (Address const& a, bool loop, bool simulator);

  public:
    void
    send_ (void const* buf, size_t s);

    ssize_t
    recv_ (void* buf, size_t s, ACE_Time_Value const* timeout);

    ssize_t
    size_ (ACE_Time_Value const* timeout);

    ACE_HANDLE
    get_handle_ () const;

  private:
    virtual void
    recv (Message_ptr m);

  private:
    bool loop_;

    Mutex mutex_;
    Condition cond_;

    ACE_Unbounded_Queue<Message_ptr> queue_;

    ACE_Pipe signal_pipe_;

    ACE_Auto_Ptr<Fragment> fragment_;
    ACE_Auto_Ptr<Reassemble> reassemble_;
    ACE_Auto_Ptr<Acknowledge> acknowledge_;
    ACE_Auto_Ptr<Retransmit> retransmit_;
    ACE_Auto_Ptr<Link> link_;
  };


  Socket_Impl::
  Socket_Impl (Address const& a, bool loop, bool simulator)
      : loop_ (loop),
        cond_ (mutex_)
  {
    signal_pipe_.open ();

    fragment_.reset (new Fragment ());
    reassemble_.reset (new Reassemble ());
    acknowledge_.reset (new Acknowledge ());
    retransmit_.reset (new Retransmit ());
    link_.reset (new Link (a, simulator));

    // Start IN stack from top to bottom.
    //
    in_start (0);
    fragment_->in_start (this);
    reassemble_->in_start (fragment_.get ());
    acknowledge_->in_start (reassemble_.get ());
    retransmit_->in_start (acknowledge_.get ());
    link_->in_start (retransmit_.get ());

    // Start OUT stack from bottom up.
    //
    link_->out_start (0);
    retransmit_->out_start (link_.get ());
    acknowledge_->out_start (retransmit_.get ());
    reassemble_->out_start (acknowledge_.get ());
    fragment_->out_start (reassemble_.get ());
    out_start (fragment_.get ());
  }

  Socket_Impl::
  ~Socket_Impl ()
  {
    // Stop OUT stack from top to bottom.
    //
    out_stop ();
    fragment_->out_stop ();
    reassemble_->out_stop ();
    acknowledge_->out_stop ();
    retransmit_->out_stop ();
    link_->out_stop ();

    // Stop IN stack from bottom up.
    //
    link_->in_stop ();
    retransmit_->in_stop ();
    acknowledge_->in_stop ();
    reassemble_->in_stop ();
    fragment_->in_stop ();
    in_stop ();
  }


  void Socket_Impl::
  send_ (void const* buf, size_t s)
  {
    Message_ptr m (new Message);

    m->add (Profile_ptr (new Data (buf, s)));

    // Qualification is for VC6 and VxWorks.
    //
    Element::send (m);
  }

  ssize_t Socket_Impl::
  recv_ (void* buf, size_t s, ACE_Time_Value const* timeout)
  {
    ACE_Time_Value abs_time;

    if (timeout)
      abs_time = ACE_OS::gettimeofday () + *timeout;

    Lock l (mutex_);

    while (queue_.is_empty ())
    {
      if (timeout)
      {
        if (cond_.wait (&abs_time) != -1)
          break;
      }
      else
      {
        if (cond_.wait () != -1)
          break;
      }

      return -1; // errno is already set
    }


    Message_ptr m;

    if (queue_.dequeue_head (m) == -1)
      abort ();


    if (queue_.is_empty ())
    {
      // Remove data from the pipe.
      //
      char c;

      if (ACE_OS::read (signal_pipe_.read_handle (), &c, 1) != 1)
      {
        perror ("read: ");
        abort ();
      }
    }


    if (m->find (NoData::id) != 0)
    {
      errno = ENOENT;
      return -1;
    }

    Data const* d = static_cast<Data const*>(m->find (Data::id));

    ssize_t r (static_cast<ssize_t> (d->size () < s ? d->size () : s));

    ACE_OS::memcpy (buf, d->buf (), r);

    return r;
  }

  ssize_t Socket_Impl::
  size_ (ACE_Time_Value const* timeout)
  {
    ACE_Time_Value abs_time;

    if (timeout)
      abs_time = ACE_OS::gettimeofday () + *timeout;

    Lock l (mutex_);

    while (queue_.is_empty ())
    {
      if (timeout)
      {
        if (cond_.wait (&abs_time) != -1)
          break;
      }
      else
      {
        if (cond_.wait () != -1)
          break;
      }

      return -1; // errno is already set
    }

    // I can't get the head of the queue without actually dequeuing
    // the element.
    //
    Message_ptr m;

    if (queue_.dequeue_head (m) == -1)
      abort ();

    if (queue_.enqueue_head (m) == -1)
      abort ();

    if (m->find (NoData::id) != 0)
    {
      errno = ENOENT;
      return -1;
    }

    Data const* d = static_cast<Data const*>(m->find (Data::id));

    return static_cast<ssize_t> (d->size ());
  }

  ACE_HANDLE Socket_Impl::
  get_handle_ () const
  {
    return signal_pipe_.read_handle ();
  }


  void Socket_Impl::
  recv (Message_ptr m)
  {
    if (m->find (Data::id) != 0 || m->find (NoData::id) != 0)
    {
      if (!loop_)
      {
        Address to (static_cast<To const*> (m->find (To::id))->address ());

        Address from (
          static_cast<From const*> (m->find (From::id))->address ());

        if (to == from)
          return;
      }

      Lock l (mutex_);

      bool signal (queue_.is_empty ());

      queue_.enqueue_tail (m);

      if (signal)
      {
        // Also write to the pipe.
        //
        char c;

        if (ACE_OS::write (signal_pipe_.write_handle (), &c, 1) != 1)
        {
          // perror ("write: ");
          abort ();
        }

        cond_.signal ();
      }

    }
  }


  // Socket
  //
  //
  Socket::
  ~Socket ()
  {
  }

  Socket::
  Socket (Address const& a, bool loop, bool simulator)
      : impl_ (new Socket_Impl (a, loop, simulator))
  {
  }

  void Socket::
  send (void const* buf, size_t s)
  {
    impl_->send_ (buf, s);
  }

  ssize_t Socket::
  recv (void* buf, size_t s)
  {
    return impl_->recv_ (buf, s, 0);
  }

  ssize_t Socket::
  recv (void* buf, size_t s, ACE_Time_Value const& timeout)
  {
    return impl_->recv_ (buf, s, &timeout);
  }

  ssize_t Socket::
  size ()
  {
    return impl_->size_ (0);
  }

  ssize_t Socket::
  size (ACE_Time_Value const& timeout)
  {
    return impl_->size_ (&timeout);
  }

  ACE_HANDLE Socket::
  get_handle () const
  {
    return impl_->get_handle_ ();
  }
}
