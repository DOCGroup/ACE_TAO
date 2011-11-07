// -*- C++ -*-
/**
 *  @file   EC_TPC_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland <cleeland at ociweb.com>
 *
 */

#ifndef TAO_EC_TPC_PROXYSUPPLIER_H
#define TAO_EC_TPC_PROXYSUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Default_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Dispatching;
class TAO_EC_TPC_Dispatching;

class TAO_RTEvent_Serv_Export TAO_EC_TPC_ProxyPushSupplier :
  public TAO_EC_Default_ProxyPushSupplier
{
public:
  /// Constructor
  TAO_EC_TPC_ProxyPushSupplier (TAO_EC_Event_Channel_Base* event_channel, int validate_connection);

  /// Dtor
  virtual ~TAO_EC_TPC_ProxyPushSupplier (void);

  /*! These are overriden from the base class in order to maintain the
       map in the dispatcher class. */

  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos);

  virtual void disconnect_push_supplier (void);

private:
  typedef TAO_EC_Default_ProxyPushSupplier BASECLASS;
  TAO_EC_TPC_Dispatching* tpc_dispatching ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_TPC_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif
