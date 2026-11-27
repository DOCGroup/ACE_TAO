/**
 *  @file   FTEC_Event_Channel.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
#ifndef FTEC_EVENT_CHANNEL_H
#define FTEC_EVENT_CHANNEL_H

#include "orbsvcs/FtRtEvent/EventChannel/ftrtec_export.h"
#include "orbsvcs/FtRtecEventChannelAdminS.h"
#include <memory>

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

  TAO_FTEC_Event_Channel (TAO_FTEC_Event_Channel const &) = delete;
  TAO_FTEC_Event_Channel (TAO_FTEC_Event_Channel &&) = delete;
  TAO_FTEC_Event_Channel & operator= (TAO_FTEC_Event_Channel const &) = delete;
  TAO_FTEC_Event_Channel & operator= (TAO_FTEC_Event_Channel &&) = delete;

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
    for_consumers () override;

  RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers () override;

  void destroy () override;

  RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer) override;

  void remove_observer (RtecEventChannelAdmin::Observer_Handle) override;

  /// EventChannelFacade Interface, used by FTEC gateway

  ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
    RtecEventComm::PushConsumer_ptr push_consumer,
    RtecEventChannelAdmin::ConsumerQOS const & qos) override;

  ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      RtecEventChannelAdmin::SupplierQOS const & qos) override;

  void disconnect_push_supplier (
      FtRtecEventChannelAdmin::ObjectId const & oid) override;

  void disconnect_push_consumer (
      FtRtecEventChannelAdmin::ObjectId const & oid) override;

  void suspend_push_supplier (
      FtRtecEventChannelAdmin::ObjectId const & oid) override;

  void resume_push_supplier (
      FtRtecEventChannelAdmin::ObjectId const & oid) override;

  void push (
      FtRtecEventChannelAdmin::ObjectId const & oid,
      RtecEventComm::EventSet const & data) override;

  /// FTRT::GroupManager Interfaces
  /// These are used for inter-replica communications

  void set_state (FTRT::State const & s) override;

  void set_update (FTRT::State const & s) override;

  void oneway_set_update (FTRT::State const & s) override;

  CORBA::Boolean start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out cur) override;

  void create_group (
      FTRT::ManagerInfoList const & info_list,
      CORBA::ULong object_group_ref_version) override;

  void join_group (FTRT::ManagerInfo const & info) override;

  void add_member (
      FTRT::ManagerInfo const & info,
      CORBA::ULong object_group_ref_version) override;

  void remove_member (
      FTRT::Location const & crashed_location,
      CORBA::ULong object_group_ref_version) override;

  void replica_crashed (FTRT::Location const & location) override;

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_, persistent_poa_;
  std::unique_ptr<TAO_FTEC_Event_Channel_Impl> ec_impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
