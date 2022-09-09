// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Event_Channel.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FTEC_EVENT_CHANNEL_H
#define FTEC_EVENT_CHANNEL_H

#include "orbsvcs/FtRtEvent/EventChannel/ftrtec_export.h"
#include "orbsvcs/FtRtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Event_Channel_Impl;
class TAO_FTEC_Become_Primary_Listener;

class TAO_FTRTEC_Export TAO_FTEC_Event_Channel
  : public POA_FtRtecEventChannelAdmin::EventChannel
{
public:
  TAO_FTEC_Event_Channel(CORBA::ORB_var orb,
                         PortableServer::POA_var poa);

  ~TAO_FTEC_Event_Channel() override;

  enum MEMBERSHIP {
    UNSPECIFIED,
    PRIMARY,
    BACKUP
  };

  /**
   * Activates the FT Event Channel.
   *
   * If the FT EC is activated as a primary, the EC will register itself to the NameService.
   * If it is activated as a backup, it will try to look up the primary from the NameService
   * and then join the object group.
   *
   * @param membership Specify the role the activated event channel in an object group.
   *
   */
  FtRtecEventChannelAdmin::EventChannel_ptr
    activate(MEMBERSHIP membership);

  /**
   * Registers a listener to accept the notication when this object
   * becomes the primary of the object group.
   */
  void set_listener(TAO_FTEC_Become_Primary_Listener* listener);


  /// RtecEventChannelAdmin::EventChannel Interface

  RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (void) override;

  RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (void) override;

  void destroy (void) override;

  RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer) override;

  void remove_observer (RtecEventChannelAdmin::Observer_Handle) override;

  /// EventChannelFacade Interface, used by FTEC gateway

  ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
    RtecEventComm::PushConsumer_ptr push_consumer,
    const RtecEventChannelAdmin::ConsumerQOS & qos) override;

  ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS & qos) override;

  void disconnect_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid) override;

  void disconnect_push_consumer (
      const FtRtecEventChannelAdmin::ObjectId & oid) override;

  void suspend_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid) override;

  void resume_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid) override;

  void push (
      const FtRtecEventChannelAdmin::ObjectId & oid,
      const RtecEventComm::EventSet & data) override;

  /// FTRT::GroupManager Interfaces
  /// These are used for inter-replica communications

  void set_state (const FTRT::State & s) override;

  void set_update (const FTRT::State & s) override;

  void oneway_set_update (const FTRT::State & s) override;


  CORBA::Boolean start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out cur) override;

  void create_group (
      const FTRT::ManagerInfoList & info_list,
      CORBA::ULong object_group_ref_version) override;

  void join_group (const FTRT::ManagerInfo & info) override;

  void add_member (
      const FTRT::ManagerInfo & info,
      CORBA::ULong object_group_ref_version) override;

  void remove_member (
      const FTRT::Location & crashed_location,
      CORBA::ULong object_group_ref_version) override;

  void replica_crashed (const FTRT::Location & location) override;

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_, persistent_poa_;
  TAO_FTEC_Event_Channel_Impl* ec_impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
