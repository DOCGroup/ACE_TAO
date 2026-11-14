/* -*- C++ -*- */

#ifndef _SUPPLIER_ROUTER_H
#define _SUPPLIER_ROUTER_H

#include "ace/UPIPE_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/UPIPE_Acceptor.h"
#include "ace/Map_Manager.h"
#include "ace/Svc_Handler.h"
#include "Peer_Router.h"

#if defined (ACE_HAS_THREADS)

class Supplier_Handler;

/// Handle type used as the supplier map key.
using SUPPLIER_KEY = ACE_HANDLE;

/// Router type that handles supplier connections.
using SUPPLIER_ROUTER = Peer_Router<Supplier_Handler, SUPPLIER_KEY>;

class Supplier_Handler
  : public Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY>
{
public:
  explicit Supplier_Handler (ACE_Thread_Manager *tm = nullptr);
  Supplier_Handler (Supplier_Handler const &) = delete;
  Supplier_Handler (Supplier_Handler &&) = delete;
  Supplier_Handler &operator= (Supplier_Handler const &) = delete;
  Supplier_Handler &operator= (Supplier_Handler &&) = delete;
  int open (void *) override;
};

/// The interface between a supplier and an Event Service ACE_Stream.
class Supplier_Router : public SUPPLIER_ROUTER
{
public:
  explicit Supplier_Router (ACE_Thread_Manager *);
  Supplier_Router (Supplier_Router const &) = delete;
  Supplier_Router (Supplier_Router &&) = delete;
  Supplier_Router &operator= (Supplier_Router const &) = delete;
  Supplier_Router &operator= (Supplier_Router &&) = delete;

protected:
  /// @name ACE_Task hooks
  /// @{

  int open (void *a = nullptr) override;
  int close (u_long flags = 0) override;
  int put (ACE_Message_Block *msg, ACE_Time_Value * = nullptr) override;
  int svc () override;

  /// @}

  /// @name Dynamic linking hooks inherited from Peer_Router
  /// @{

  int info (ACE_TCHAR **info_string, size_t length) const override;

  /// @}
};

#endif /* ACE_HAS_THREADS */
#endif /* _SUPPLIER_ROUTER_H */
