/* -*- C++ -*- */
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
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_ProxySupplier.h"

/**
 * @class TAO_EC_Default_ProxyPushSupplier
 *
 * @brief Implement the ReliableEventChannelAdmin::ProxyPushSupplier
 *         interface
 */
class TAO_RTEvent_Export TAO_EC_Default_ProxyPushSupplier :
  public POA_RtecEventChannelAdmin::ProxyPushSupplier,
  public TAO_EC_ProxyPushSupplier
{
public:

  /// Constructor...
  TAO_EC_Default_ProxyPushSupplier (TAO_EC_Event_Channel_Base* event_channel, int validate_connection);

  /// Destructor...
  virtual ~TAO_EC_Default_ProxyPushSupplier (void);

  void activate (
     RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// IDL methods.
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos
                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected,
                       RtecEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void suspend_connection (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void resume_connection (ACE_ENV_SINGLE_ARG_DECL)
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
#include "EC_Default_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_DEFAULT_PROXYSUPPLIER_H */
