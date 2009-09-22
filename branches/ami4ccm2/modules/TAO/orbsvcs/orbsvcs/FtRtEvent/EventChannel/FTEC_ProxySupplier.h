// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FTEC_PROXYSUPPLIER_H
#define TAO_FTEC_PROXYSUPPLIER_H
#include "orbsvcs/Event/EC_Default_ProxySupplier.h"
#include "ace/SString.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_ProxyPushSupplier : public TAO_EC_Default_ProxyPushSupplier
{
  typedef TAO_EC_Default_ProxyPushSupplier Inherited;
public:
  typedef RtecEventChannelAdmin::ProxyPushSupplier Interface;
  typedef FtRtecEventChannelAdmin::ProxyPushSupplierStat State;
  typedef POA_RtecEventChannelAdmin::ProxyPushSupplier_ptr Skeleton;

  TAO_FTEC_ProxyPushSupplier(TAO_EC_Event_Channel_Base* event_channel,
                           int validate_connection);
    /// Activate in the POA
 virtual void activate (
       RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy);
    // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos);
  virtual void disconnect_push_supplier (void);
  virtual void suspend_connection (void);
  virtual void resume_connection (void);

  const FtRtecEventChannelAdmin::ObjectId& id() const;

  void get_state(FtRtecEventChannelAdmin::ProxyPushSupplierStat& state);
  void set_state(const FtRtecEventChannelAdmin::ProxyPushSupplierStat& state);

  typedef void (FtRtecEventChannelAdmin::EventChannelFacade::*RollbackOperation)
    (const FtRtecEventChannelAdmin::ObjectId&);

  static const RollbackOperation rollback_obtain;
private:
  FtRtecEventChannelAdmin::ObjectId_var object_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
