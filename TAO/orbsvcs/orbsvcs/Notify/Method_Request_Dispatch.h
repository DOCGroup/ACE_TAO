// -*- C++ -*-

/**
 *  @file Method_Request_Dispatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_DISPATCH_METHOD_REQUEST_H
#define TAO_Notify_DISPATCH_METHOD_REQUEST_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Method_Request.h"
#include "orbsvcs/Notify/Refcountable.h"
#include "orbsvcs/Notify/Method_Request_Event.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Delivery_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_EventChannelFactory;
class TAO_InputCDR;
class TAO_Notify_Method_Request_Dispatch_Queueable;

/**
 * @class TAO_Notify_Method_Request_Dispatch
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch
  : public TAO_Notify_Method_Request_Event
{
public:
  /// an arbitrary code (Octet) to identify this delivery method type in persistent storage
  enum {persistence_code = 1};

  /// Constuct from event
  TAO_Notify_Method_Request_Dispatch (
    const TAO_Notify_Event * event,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

  /// Construct from a delivery rquest
  TAO_Notify_Method_Request_Dispatch (
    const TAO_Notify::Delivery_Request_Ptr & delivery,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

  /// Constuct from another method request+event
  /// event is passed separately because we may be using a copy
  /// of the one in the previous method request
  TAO_Notify_Method_Request_Dispatch (
    const TAO_Notify_Method_Request_Event & request,
    const TAO_Notify_Event * event,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

public:
  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch ();

  /// Static method used to reconstruct a Method Request Dispatch
  static TAO_Notify_Method_Request_Dispatch_Queueable * unmarshal (
    TAO_Notify::Delivery_Request_Ptr & delivery_request,
    TAO_Notify_EventChannelFactory &ecf,
    TAO_InputCDR & cdr);


protected:
  /// Execute the dispatch operation.
  int execute_i (void);

protected:
  /// The Proxy
  TAO_Notify_ProxySupplier::Ptr proxy_supplier_;

  /// Flag is true if we want to do filtering else false.
  bool filtering_;
};

/**
 * @class TAO_Notify_Method_Request_Dispatch_Queueable
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */

class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch_Queueable
    : public TAO_Notify_Method_Request_Dispatch
    , public TAO_Notify_Method_Request_Queueable
{
public:
  /// Construct construct from another method request+event
  /// event is passed separately because we may be using a copy
  /// of the one in the previous method request
  TAO_Notify_Method_Request_Dispatch_Queueable (
    const TAO_Notify_Method_Request_Event & request,
    TAO_Notify_Event::Ptr & event,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

  /// Constuct construct from Delivery Request
  /// should ONLY be used by unmarshall
  TAO_Notify_Method_Request_Dispatch_Queueable (
    const TAO_Notify::Delivery_Request_Ptr & request,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch_Queueable ();

  /// Execute the Request
  virtual int execute (void);

private:
  TAO_Notify_Event::Ptr event_var_;
  TAO_Notify_ProxySupplier::Ptr proxy_guard_;
};

/*****************************************************************************/

/**
 * @class TAO_Notify_Method_Request_Dispatch_No_Copy
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Dispatch_No_Copy
    : public TAO_Notify_Method_Request_Dispatch
    , public TAO_Notify_Method_Request
{
public:
  /// Constuct construct from another method request
  TAO_Notify_Method_Request_Dispatch_No_Copy (
    const TAO_Notify_Method_Request_Event & request,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch_No_Copy ();

  /// Execute the Request
  virtual int execute (void);

  /// Create a copy of this method request
  virtual TAO_Notify_Method_Request_Queueable* copy (void);
};

/*****************************************************************************/

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DISPATCH_METHOD_REQUEST_H */
