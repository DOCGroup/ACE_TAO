/* -*- C++ -*- */
/**
 *  @file Method_Request_Lookup.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_LOOKUP_METHOD_REQUEST_H
#define TAO_Notify_LOOKUP_METHOD_REQUEST_H
#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "Method_Request_Event.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"
#include "Consumer_Map.h"
#include "Delivery_Request.h"

class TAO_Notify_Method_Request_Lookup_Queueable;
class TAO_Notify_Event;

/**
 * @class TAO_Notify_Method_Request_Lookup
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Lookup
  : public TAO_ESF_Worker<TAO_Notify_ProxySupplier>
  , public TAO_Notify_Method_Request_Event
{
public:

  /// an arbitrary code (Octet) to identify this type of request in persistent storage
  enum {persistence_code = 2};

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Lookup ();

  /// Static method used to reconstruct a Method Request Dispatch
  static TAO_Notify_Method_Request_Lookup_Queueable * unmarshal (
    TAO_Notify::Delivery_Request_Ptr & delivery_request,
    TAO_Notify_EventChannelFactory &ecf,
    TAO_InputCDR & cdr
    ACE_ENV_ARG_DECL);

protected:
  /// Constuctor
  TAO_Notify_Method_Request_Lookup (const TAO_Notify_Event * event, TAO_Notify_ProxyConsumer * proxy);

  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

  ///= TAO_ESF_Worker method
  virtual void work (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

protected:

  /// The Proxy
  TAO_Notify_ProxyConsumer* proxy_consumer_;
};

/***************************************************************/

/**
 * @class TAO_Notify_Method_Request_Lookup_Queueable
 *
 * @brief Lookup command object looks up the event type of the given event in the consumer map and send the event to each proxysupplier.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Lookup_Queueable
  : public TAO_Notify_Method_Request_Lookup
  , public TAO_Notify_Method_Request_Queueable
{
public:
  /// Constuctor from event
  TAO_Notify_Method_Request_Lookup_Queueable (
    const TAO_Notify_Event_var& event,
    TAO_Notify_ProxyConsumer * proxy_consumer);

  /// Constuctor from delivery request
  TAO_Notify_Method_Request_Lookup_Queueable (
    TAO_Notify::Delivery_Request_Ptr & request,
    TAO_Notify_ProxyConsumer * proxy_consumer);

  /// Destructor
  ~TAO_Notify_Method_Request_Lookup_Queueable ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

private:
  const TAO_Notify_Event_var event_var_;
  TAO_Notify_ProxyConsumer_Guard proxy_guard_;
};

/*****************************************************************************************************************************/

/**
 * @class TAO_Notify_Method_Request_Lookup_No_Copy
 *
 * @brief Lookup command object looks up the event type of the given event in the consumer map and send the event to each proxysupplier.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Lookup_No_Copy
  : public TAO_Notify_Method_Request_Lookup
  , public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Lookup_No_Copy (
    const TAO_Notify_Event* event,
    TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Destructor
  ~TAO_Notify_Method_Request_Lookup_No_Copy ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request_Queueable* copy (ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_LOOKUP_METHOD_REQUEST_H */
