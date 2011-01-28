// -*- C++ -*-

/**
 *  @file   EC_Default_ProxyConsumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 *  @author Jason Smith (jason@atdesk.com)
 *
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
 *
 */
class TAO_RTEvent_Serv_Export TAO_EC_Default_ProxyPushConsumer :
  public POA_RtecEventChannelAdmin::ProxyPushConsumer,
  public TAO_EC_ProxyPushConsumer
{
public:

  /// Constructor...
  TAO_EC_Default_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  /// Destructor...
  virtual ~TAO_EC_Default_ProxyPushConsumer (void);

 virtual void activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy);

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void push (const RtecEventComm::EventSet& event);
  virtual void disconnect_push_consumer (void);

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

#endif /* TAO_EC_DEFAULT_PROXYCONSUMER_H */
