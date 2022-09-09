// -*- C++ -*-

/**
 *  @file   EC_Default_ProxyConsumer.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 *  @author Jason Smith (jason@atdesk.com)
 */

#ifndef TAO_EC_DEFAULT_PROXYCONSUMER_H
#define TAO_EC_DEFAULT_PROXYCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_ProxyConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_Supplier_Filter;

/**
 * @class TAO_EC_Default_ProxyPushConsumer
 *
 * @brief implements RtecEventChannelAdmin::ProxyPushConsumer interface.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Default_ProxyPushConsumer :
  public POA_RtecEventChannelAdmin::ProxyPushConsumer,
  public TAO_EC_ProxyPushConsumer
{
public:
  /// Constructor...
  TAO_EC_Default_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  /// Destructor...
  ~TAO_EC_Default_ProxyPushConsumer () override;

 void activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy) override;

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos) override;
  void push (const RtecEventComm::EventSet& event) override;
  void disconnect_push_consumer () override;

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

#endif /* TAO_EC_DEFAULT_PROXYCONSUMER_H */
