// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_ProxyConsumer.h
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
  using Inherited = TAO_EC_Default_ProxyPushConsumer;
public:
  using Interface = RtecEventChannelAdmin::ProxyPushConsumer;
  using State = FtRtecEventChannelAdmin::ProxyPushConsumerStat;
  using Skeleton = POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr;

  TAO_FTEC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  virtual void activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy) override;

    // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                RtecEventChannelAdmin::SupplierQOS const& qos) override;
  void disconnect_push_consumer () override;

  const FtRtecEventChannelAdmin::ObjectId& id() const;
  void get_state(FtRtecEventChannelAdmin::ProxyPushConsumerStat& state);
  void set_state(const FtRtecEventChannelAdmin::ProxyPushConsumerStat& state);

  using RollbackOperation = void (FtRtecEventChannelAdmin::EventChannelFacade::*)
    (FtRtecEventChannelAdmin::ObjectId const&);

  static const RollbackOperation rollback_obtain;
private:
  FtRtecEventChannelAdmin::ObjectId_var object_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
