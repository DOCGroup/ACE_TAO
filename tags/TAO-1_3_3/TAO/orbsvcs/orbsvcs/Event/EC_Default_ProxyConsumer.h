/* -*- C++ -*- */
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
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_ProxyConsumer.h"

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_Supplier_Filter;

/**
 * @class TAO_EC_Default_ProxyPushConsumer
 *
 * @brief implements RtecEventChannelAdmin::ProxyPushConsumer interface.
 *
 */
class TAO_RTEvent_Export TAO_EC_Default_ProxyPushConsumer :
  public POA_RtecEventChannelAdmin::ProxyPushConsumer,
  public TAO_EC_ProxyPushConsumer
{
public:

  /// Constructor...
  TAO_EC_Default_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  /// Destructor...
  virtual ~TAO_EC_Default_ProxyPushConsumer (void);

  void activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos
                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecEventChannelAdmin::AlreadyConnected));
  virtual void push (const RtecEventComm::EventSet& event
                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

private:

  virtual PortableServer::ObjectId
         object_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};


#if defined (__ACE_INLINE__)
#include "EC_Default_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_DEFAULT_PROXYCONSUMER_H */
