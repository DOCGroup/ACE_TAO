// -*- C++ -*-

/**
 *  @file   EC_Default_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 *  @author Jason Smith (jason@atdesk.com)
 */

#ifndef TAO_DEFAULT_EC_PROXYSUPPLIER_H
#define TAO_DEFAULT_EC_PROXYSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_ProxySupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_Default_ProxyPushSupplier
 *
 * @brief Implement the ReliableEventChannelAdmin::ProxyPushSupplier
 *         interface
 */
class TAO_RTEvent_Serv_Export TAO_EC_Default_ProxyPushSupplier
  : public POA_RtecEventChannelAdmin::ProxyPushSupplier,
    public TAO_EC_ProxyPushSupplier
{
public:

  /// Constructor...
  TAO_EC_Default_ProxyPushSupplier (TAO_EC_Event_Channel_Base* event_channel, int validate_connection);

  /// Destructor...
  virtual ~TAO_EC_Default_ProxyPushSupplier (void);

  void activate (
     RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy);

  /// IDL methods.
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos);
  virtual void disconnect_push_supplier (void);
  virtual void suspend_connection (void);
  virtual void resume_connection (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (void);
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

private:

  virtual PortableServer::ObjectId
            object_id (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_EC_DEFAULT_PROXYSUPPLIER_H */
