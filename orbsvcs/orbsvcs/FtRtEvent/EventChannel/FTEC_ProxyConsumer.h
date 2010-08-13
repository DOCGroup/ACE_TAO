// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_ProxyConsumer.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FTEC_PROXYCONSUMER_H
#define TAO_FTEC_PROXYCONSUMER_H

#include "orbsvcs/Event/EC_Default_ProxyConsumer.h"
#include "ace/SString.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_ProxyPushConsumer : public TAO_EC_Default_ProxyPushConsumer
{
  typedef TAO_EC_Default_ProxyPushConsumer Inherited;
public:
  typedef RtecEventChannelAdmin::ProxyPushConsumer Interface;
  typedef FtRtecEventChannelAdmin::ProxyPushConsumerStat State;
  typedef POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr Skeleton;

  TAO_FTEC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  virtual void activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy);


    // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void disconnect_push_consumer (void);

  const FtRtecEventChannelAdmin::ObjectId& id() const;
  void get_state(FtRtecEventChannelAdmin::ProxyPushConsumerStat& state);
  void set_state(const FtRtecEventChannelAdmin::ProxyPushConsumerStat& state);

  typedef void (FtRtecEventChannelAdmin::EventChannelFacade::*RollbackOperation)
    (const FtRtecEventChannelAdmin::ObjectId&);

  static const RollbackOperation rollback_obtain;
private:
  FtRtecEventChannelAdmin::ObjectId_var object_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
