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

#ifndef TAO_NS_LOOKUP_METHOD_REQUEST_H
#define TAO_NS_LOOKUP_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "Method_Request.h"
#include "ProxySupplier.h"
#include "Consumer_Map.h"

class TAO_NS_ProxyConsumer;

/**
 * @class TAO_NS_Method_Request_Lookup
 *
 * @brief Lookup command object looks up the event type of the given event in the consumer map and send the event to each proxysupplier.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Lookup : public TAO_NS_Method_Request_Event, public TAO_ESF_Worker<TAO_NS_ProxySupplier>
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Lookup (const TAO_NS_Event_var& event, TAO_NS_ProxyConsumer* proxy_consumer, TAO_NS_Consumer_Map* map);

  /// Destructor
  ~TAO_NS_Method_Request_Lookup ();

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  ///= TAO_ESF_Worker method
  void work (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

private:
  /// ProxyConsumer supplying event.
  TAO_NS_ProxyConsumer* proxy_consumer_;

  /// The map of subscriptions.
  TAO_NS_Consumer_Map* map_;

  /// Guard to automatically inc/decr ref count on the proxy.
  TAO_NS_Refcountable_Guard refcountable_guard_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_LOOKUP_METHOD_REQUEST_H */
