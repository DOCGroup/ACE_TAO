// -*- C++ -*-

/**
 * @file EC_Proxy_Disconnector.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 * $Id$
 */
#ifndef EC_PROXY_DISCONNECTOR__H_
#define EC_PROXY_DISCONNECTOR__H_

#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/RtecEventCommC.h"
#include "ace/Auto_Functor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @struct TAO_EC_Supplier_Proxy_Disconnect_Functor
 *
 * @brief Implements a functor for the TAO_EC_Supplier_Proxy_Disconnector class.
 */
struct TAO_RTEvent_Serv_Export TAO_EC_Supplier_Proxy_Disconnect_Functor
{
  typedef  RtecEventComm::PushSupplier_ptr argument;

  /// Disconnect from @c supplier
  void operator() (RtecEventComm::PushSupplier_ptr supplier) throw ();
};

/**
 * @class TAO_EC_Supplier_Proxy_Disconnector
 *
 * @brief Helper class to disconnect a PushConsumer from the event
 *        channel.
 *
 */
typedef ACE_Utils::Auto_Functor<
            RtecEventComm::PushSupplier,
            TAO_EC_Supplier_Proxy_Disconnect_Functor>
        TAO_EC_Supplier_Proxy_Disconnector;



/**
 * @struct TAO_EC_Consumer_Proxy_Disconnect_Functor
 *
 * @brief Implements a functor for the TAO_EC_Consumer_Proxy_Disconnector class.
 */
struct TAO_RTEvent_Serv_Export TAO_EC_Consumer_Proxy_Disconnect_Functor
{
  typedef  RtecEventComm::PushConsumer_ptr argument;

  /// Disconnect from @c consumer
  void operator() (RtecEventComm::PushConsumer_ptr consumer) throw ();
};

/**
 * @class TAO_EC_Consumer_Proxy_Disconnector
 *
 * @brief Helper class to disconnect a PushSupplier from the event
 *        channel.
 *
 */
typedef ACE_Utils::Auto_Functor<
            RtecEventComm::PushConsumer,
            TAO_EC_Consumer_Proxy_Disconnect_Functor>
        TAO_EC_Consumer_Proxy_Disconnector;


TAO_END_VERSIONED_NAMESPACE_DECL

#endif // EC_PROXY_DISCONNECTOR__H_
