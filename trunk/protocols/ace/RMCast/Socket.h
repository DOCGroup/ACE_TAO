// -*- C++ -*-

// file      : ace/RMCast/Socket.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_SOCKET_H
#define ACE_RMCAST_SOCKET_H

#include "ace/Unbounded_Queue.h"
#include "ace/Thread_Mutex.h"
#include "ace/Condition_T.h"

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

#include "Link.h"
#include "Simulator.h"
#include "Retransmit.h"
#include "Acknowledge.h"


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
