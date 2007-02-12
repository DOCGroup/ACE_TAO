// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Event_Channel.h
 *
 *  $Id$
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

  ~TAO_FTEC_Event_Channel();

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

  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (void);

  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (void);

  virtual void destroy (void);

  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer);

  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle);

  /// EventChannelFacade Interface, used by FTEC gateway

  virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
    RtecEventComm::PushConsumer_ptr push_consumer,
    const RtecEventChannelAdmin::ConsumerQOS & qos);

  virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS & qos);

  virtual void disconnect_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid);

  virtual void disconnect_push_consumer (
      const FtRtecEventChannelAdmin::ObjectId & oid);

  virtual void suspend_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid);

  virtual void resume_push_supplier (
      const FtRtecEventChannelAdmin::ObjectId & oid);

  virtual void push (
      const FtRtecEventChannelAdmin::ObjectId & oid,
      const RtecEventComm::EventSet & data);

  /// FTRT::GroupManager Interfaces
  /// These are used for inter-replica communications

  void set_state (const FTRT::State & s);

  virtual void set_update (const FTRT::State & s);

  virtual void oneway_set_update (const FTRT::State & s);


  virtual CORBA::Boolean start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out cur);

  virtual void create_group (
      const FTRT::ManagerInfoList & info_list,
      CORBA::ULong object_group_ref_version);

  virtual void join_group (const FTRT::ManagerInfo & info);

  virtual void add_member (
      const FTRT::ManagerInfo & info,
      CORBA::ULong object_group_ref_version);

  virtual void remove_member (
      const FTRT::Location & crashed_location,
      CORBA::ULong object_group_ref_version);

  virtual void replica_crashed (const FTRT::Location & location);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_, persistent_poa_;
  TAO_FTEC_Event_Channel_Impl* ec_impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
