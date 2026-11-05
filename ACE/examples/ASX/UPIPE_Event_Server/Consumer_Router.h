/* -*- C++ -*- */
// The interface between one or more consumers and an Event Server
// ACE_Stream.

#ifndef _CONSUMER_ROUTER_H
#define _CONSUMER_ROUTER_H

#include "ace/Thread_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Addr.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

#if defined (ACE_HAS_THREADS)

class Consumer_Handler; // Forward declaration....

using CONSUMER_KEY = ACE_HANDLE;

using CONSUMER_ROUTER = Peer_Router<Consumer_Handler, CONSUMER_KEY>;

class Consumer_Handler
  : public Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY>
{
public:
  explicit Consumer_Handler (ACE_Thread_Manager *tm = nullptr);
  Consumer_Handler (Consumer_Handler const &) = delete;
  Consumer_Handler (Consumer_Handler &&) = delete;
  Consumer_Handler &operator= (Consumer_Handler const &) = delete;
  Consumer_Handler &operator= (Consumer_Handler &&) = delete;
  int open (void *) override;
};

class Consumer_Router : public CONSUMER_ROUTER
{
public:
  explicit Consumer_Router (ACE_Thread_Manager *thr_manager);
  Consumer_Router (Consumer_Router const &) = delete;
  Consumer_Router (Consumer_Router &&) = delete;
  Consumer_Router &operator= (Consumer_Router const &) = delete;
  Consumer_Router &operator= (Consumer_Router &&) = delete;

protected:
  // ACE_Task hooks..
  int open (void *a = nullptr) override;
  int close (u_long flags = 0) override;
  int put (ACE_Message_Block *msg, ACE_Time_Value * = nullptr) override;
  int svc () override;

  // Dynamic linking hooks.
  int info (ACE_TCHAR **info_string, size_t length) const override;
};
#endif /* ACE_HAS_THREADS */
#endif /* _CONSUMER_ROUTER_H */
