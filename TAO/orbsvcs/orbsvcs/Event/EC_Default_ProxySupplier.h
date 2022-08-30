// -*- C++ -*-

/**
 *  @file   EC_Default_ProxySupplier.h
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
  ~TAO_EC_Default_ProxyPushSupplier () override;

  void activate (
     RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy) override;

  /// IDL methods.
  void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos) override;
  void disconnect_push_supplier () override;
  void suspend_connection () override;
  void resume_connection () override;

  // = The Servant methods
  PortableServer::POA_ptr _default_POA () override;
  void _add_ref () override;
  void _remove_ref () override;

private:

  PortableServer::ObjectId
            object_id () override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_EC_DEFAULT_PROXYSUPPLIER_H */
