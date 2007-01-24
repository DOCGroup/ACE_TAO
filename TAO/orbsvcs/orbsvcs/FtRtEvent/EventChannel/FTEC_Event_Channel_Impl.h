// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Event_Channel_Impl.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FTEC_EVENT_CHANNEL_H
#define TAO_FTEC_EVENT_CHANNEL_H

#include "ace/Synch_T.h"
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
  TAO_FTEC_Event_Channel_Impl(const TAO_EC_Event_Channel_Attributes& attributes);

  ~TAO_FTEC_Event_Channel_Impl();

  /// Start the internal threads (if any), etc.
  /// After this call the EC can be used.
  virtual void activate_object (
    CORBA::ORB_var orb,
    const FtRtecEventComm::ObjectId& supplier_admin_oid,
    const FtRtecEventComm::ObjectId& consumer_admin_oid
    ACE_ENV_ARG_DECL);


  // = The RtecEventChannelAdmin::EventChannel methods...
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
      for_consumers (void)
        ACE_THROW_SPEC ((CORBA::SystemException));


  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
      for_suppliers (void)
        ACE_THROW_SPEC ((CORBA::SystemException));

  void get_state (FtRtecEventChannelAdmin::EventChannelState & s
                  ACE_ENV_ARG_DECL);

  void set_state (const FTRT::State & stat ACE_ENV_ARG_DECL);

  void set_update (const FTRT::State & s ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException, FTRT::InvalidUpdate));

  /// EventChannelFacade Interface

   virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        const RtecEventChannelAdmin::ConsumerQOS & qos
        ACE_ENV_ARG_DECL
      );


    virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        const RtecEventChannelAdmin::SupplierQOS & qos
        ACE_ENV_ARG_DECL
      );

    virtual void disconnect_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      );

    virtual void disconnect_push_consumer (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      );

    virtual void suspend_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      );

    virtual void resume_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      );

    virtual void push (
        const FtRtecEventChannelAdmin::ObjectId & oid,
        const RtecEventComm::EventSet & data
        ACE_ENV_ARG_DECL
      );

  ////////////////////////////////////////////

    /// Access the consumer admin implementation, useful for controlling
  /// the activation...
  TAO_FTEC_ConsumerAdmin* consumer_admin (void) const;

  /// Access the supplier admin implementation, useful for controlling
  /// the activation...
  TAO_FTEC_SupplierAdmin* supplier_admin (void) const;

  TAO_FTEC_ProxyPushSupplier* find_proxy_push_supplier(const FtRtecEventChannelAdmin::ObjectId& id);
  TAO_FTEC_ProxyPushConsumer* find_proxy_push_consumer(const FtRtecEventChannelAdmin::ObjectId& id);


  CORBA::Object_var make_iogr(CORBA::Object_ptr obj
                              ACE_ENV_ARG_DECL);

protected:
  TAO_FTEC_Basic_Factory* factory();
private:
  IOGR_Maker iogr_maker_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
