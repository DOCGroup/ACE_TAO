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

#include "RMCast_Export.h"


namespace ACE_RMCast
{
  class ACE_RMCast_Export Socket : protected Element
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
    virtual void
    recv (Message_ptr m);

  private:
    bool loop_;

    u64 sn_; //@@ lock?

    Mutex mutex_;
    Condition cond_;

    ACE_Unbounded_Queue<Message_ptr> queue_;

    ACE_Auto_Ptr<Acknowledge> acknowledge_;
    ACE_Auto_Ptr<Retransmit> retransmit_;
    ACE_Auto_Ptr<Simulator> simulator_;
    ACE_Auto_Ptr<Link> link_;
  };
}


#endif  // ACE_RMCAST_SOCKET_H
