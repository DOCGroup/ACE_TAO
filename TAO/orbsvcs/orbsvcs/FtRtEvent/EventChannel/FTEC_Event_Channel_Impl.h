/**
 *  @file   FTEC_Event_Channel_Impl.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
#ifndef TAO_FTEC_EVENT_CHANNEL_H
#define TAO_FTEC_EVENT_CHANNEL_H

#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Factory.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Group_Manager.h"
#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_ProxyPushSupplier;
class TAO_FTEC_ProxyPushConsumer;
class TAO_FTEC_SupplierAdmin;
class TAO_FTEC_ConsumerAdmin;
class FT_ProxyAdmin_Base;

class TAO_FTEC_Event_Channel_Impl : public TAO_EC_Event_Channel_Base
                             , public TAO_FTEC_Group_Manager
{
public:
  explicit TAO_FTEC_Event_Channel_Impl(TAO_EC_Event_Channel_Attributes const &attributes);

  ~TAO_FTEC_Event_Channel_Impl() override = default;

  TAO_FTEC_Event_Channel_Impl(TAO_FTEC_Event_Channel_Impl const&) = delete;
  TAO_FTEC_Event_Channel_Impl(TAO_FTEC_Event_Channel_Impl&&) = delete;
  TAO_FTEC_Event_Channel_Impl& operator=(TAO_FTEC_Event_Channel_Impl const&) = delete;
  TAO_FTEC_Event_Channel_Impl& operator=(TAO_FTEC_Event_Channel_Impl&&) = delete;

  /// Start the internal threads (if any), etc.
  /// After this call the EC can be used.
  virtual void activate_object (
    CORBA::ORB_var orb,
    FtRtecEventComm::ObjectId const& supplier_admin_oid,
    FtRtecEventComm::ObjectId const& consumer_admin_oid);

  /// @name RtecEventChannelAdmin::EventChannel methods
  /// @{
  RtecEventChannelAdmin::ConsumerAdmin_ptr
      for_consumers () override;

  RtecEventChannelAdmin::SupplierAdmin_ptr
      for_suppliers () override;

  void get_state (FtRtecEventChannelAdmin::EventChannelState &s) override;
  /// @}

  void set_state (FTRT::State const & stat);

  void set_update (FTRT::State const & s);

  /// EventChannelFacade Interface

   virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        RtecEventChannelAdmin::ConsumerQOS const & qos);

    virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        RtecEventChannelAdmin::SupplierQOS const & qos);

    virtual void disconnect_push_supplier (
        FtRtecEventChannelAdmin::ObjectId const & oid);

    virtual void disconnect_push_consumer (
        FtRtecEventChannelAdmin::ObjectId const & oid);

    virtual void suspend_push_supplier (
        FtRtecEventChannelAdmin::ObjectId const & oid);

    virtual void resume_push_supplier (
        FtRtecEventChannelAdmin::ObjectId const & oid);

    virtual void push (
        FtRtecEventChannelAdmin::ObjectId const & oid,
        RtecEventComm::EventSet const & data);

  ////////////////////////////////////////////

    /// Access the consumer admin implementation, useful for controlling
  /// the activation...
  TAO_FTEC_ConsumerAdmin* consumer_admin () const;

  /// Access the supplier admin implementation, useful for controlling
  /// the activation...
  TAO_FTEC_SupplierAdmin* supplier_admin () const;

  TAO_FTEC_ProxyPushSupplier* find_proxy_push_supplier(FtRtecEventChannelAdmin::ObjectId const& id);
  TAO_FTEC_ProxyPushConsumer* find_proxy_push_consumer(FtRtecEventChannelAdmin::ObjectId const& id);

  CORBA::Object_var make_iogr(CORBA::Object_ptr obj);

protected:
  TAO_FTEC_Basic_Factory* factory();
private:
  IOGR_Maker iogr_maker_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
