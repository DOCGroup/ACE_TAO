// -*- C++ -*-

/**
 *  @file Method_Request_Lookup.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */
#ifndef TAO_Notify_LOOKUP_METHOD_REQUEST_H
#define TAO_Notify_LOOKUP_METHOD_REQUEST_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/Notify/Method_Request_Event.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/Consumer_Map.h"
#include "orbsvcs/Notify/Delivery_Request.h"

#include "orbsvcs/ESF/ESF_Worker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    TAO_InputCDR & cdr);

protected:
  /// Constructor
  TAO_Notify_Method_Request_Lookup (const TAO_Notify_Event * event, TAO_Notify_ProxyConsumer * proxy);

  /// Constructor
  TAO_Notify_Method_Request_Lookup (const TAO_Notify::Delivery_Request_Ptr& delivery, TAO_Notify_ProxyConsumer * proxy);

  /// Execute the dispatch operation.
  int execute_i (void);

  ///= TAO_ESF_Worker method
  virtual void work (TAO_Notify_ProxySupplier* proxy_supplier);

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
  /// Constructor from event
  TAO_Notify_Method_Request_Lookup_Queueable (
    const TAO_Notify_Event::Ptr& event,
    TAO_Notify_ProxyConsumer * proxy_consumer);

  /// Constructor from delivery request
  TAO_Notify_Method_Request_Lookup_Queueable (
    TAO_Notify::Delivery_Request_Ptr & request,
    TAO_Notify_ProxyConsumer * proxy_consumer);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Lookup_Queueable ();

  /// Execute the Request
  virtual int execute (void);

private:
  TAO_Notify_Event::Ptr event_var_;
  TAO_Notify_ProxyConsumer::Ptr proxy_guard_;
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
  /// Constructor
  TAO_Notify_Method_Request_Lookup_No_Copy (
    const TAO_Notify_Event* event,
    TAO_Notify_ProxyConsumer* proxy_consumer);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Lookup_No_Copy ();

  /// Execute the Request
  virtual int execute (void);

  /// Create a copy of this object.
  virtual TAO_Notify_Method_Request_Queueable* copy (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_LOOKUP_METHOD_REQUEST_H */
