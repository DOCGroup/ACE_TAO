// -*- C++ -*-

// file      : ace/RMCast/Socket.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_SOCKET_H
#define ACE_RMCAST_SOCKET_H

#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"

#include "RMCast_Export.h"


namespace ACE_RMCast
{
  class Socket_Impl;

  class ACE_RMCast_Export Socket
  {
  public:
    virtual
    ~Socket ();

    Socket (ACE_INET_Addr const& a, bool loop = true);

  public:
    virtual void
    send (void const* buf, size_t s);

    virtual size_t
    recv (void* buf, size_t s);

  private:
    ACE_Auto_Ptr<Socket_Impl> impl_;
  };
}


#endif  // ACE_RMCAST_SOCKET_H
