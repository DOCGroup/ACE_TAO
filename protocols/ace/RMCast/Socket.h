// file      : ace/RMCast/Socket.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_SOCKET_H
#define ACE_RMCAST_SOCKET_H

#include <ace/Unbounded_Queue.h>

#include <ace/RMCast/Stack.h>
#include <ace/RMCast/Protocol.h>
#include <ace/RMCast/Bits.h>

#include <ace/RMCast/Link.h>
#include <ace/RMCast/Simulator.h>
#include <ace/RMCast/Retransmit.h>
#include <ace/RMCast/Acknowledge.h>

namespace ACE_RMCast
{
  class Socket : protected Element
  {
  public:
    ~Socket ();

    Socket (Address const& a, bool loop = true);

  public:
    virtual void
    send (void const* buf, size_t s);

    virtual size_t
    recv (void* buf, size_t s);

  private:
    using Element::send;

    virtual void
    recv (Message_ptr m);

  private:
    bool loop_;

    u64 sn_; //@@ lock?

    Mutex mutex_;
    Condition cond_;

    ACE_Unbounded_Queue<Message_ptr> queue_;

    auto_ptr<Acknowledge> acknowledge_;
    auto_ptr<Retransmit> retransmit_;
    auto_ptr<Simulator> simulator_;
    auto_ptr<Link> link_;
  };
}


#endif  // ACE_RMCAST_SOCKET_H
