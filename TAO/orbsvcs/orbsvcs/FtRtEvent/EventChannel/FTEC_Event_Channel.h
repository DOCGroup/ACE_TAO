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

#include "ftrtec_export.h"
#include "orbsvcs/FtRtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
    NONE,
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
    activate(MEMBERSHIP membership ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Registers a listener to accept the notication when this object
   * becomes the primary of the object group.
   */
  void set_listener(TAO_FTEC_Become_Primary_Listener* listener);


  /// RtecEventChannelAdmin::EventChannel Interface

  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
      RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));

  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
      RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));

  /// EventChannelFacade Interface, used by FTEC gateway

      virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        const RtecEventChannelAdmin::ConsumerQOS & qos
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException, RtecEventChannelAdmin::TypeError)) ;


    virtual ::FtRtecEventChannelAdmin::ObjectId * connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        const RtecEventChannelAdmin::SupplierQOS & qos
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) ;

    virtual void disconnect_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void disconnect_push_consumer (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) ;

    virtual void suspend_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException, FtRtecEventComm::InvalidObjectID));

    virtual void resume_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException, FtRtecEventComm::InvalidObjectID)) ;

    virtual void push (
        const FtRtecEventChannelAdmin::ObjectId & oid,
        const RtecEventComm::EventSet & data
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException, FtRtecEventComm::InvalidObjectID));

  /// FTRT::GroupManager Interfaces
  /// These are used for inter-replica communications

  void set_state (
    const FTRT::State & s ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, FTRT::InvalidState));

  virtual void set_update (const FTRT::State & s ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, FTRT::InvalidUpdate, FTRT::OutOfSequence));

  virtual void oneway_set_update (const FTRT::State & s ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));


  virtual CORBA::Boolean start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out cur
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void create_group (
        const FTRT::ManagerInfoList & info_list,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException, FTRT::PredecessorUnreachable)) ;

    virtual void join_group (
        const FTRT::ManagerInfo & info
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )ACE_THROW_SPEC ((CORBA::SystemException)) ;

    virtual void add_member (
        const FTRT::ManagerInfo & info,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) ;

    virtual void remove_member (
        const FTRT::Location & crashed_location,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void replica_crashed (
        const FTRT::Location & location
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) ;


private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_, persistent_poa_;
  TAO_FTEC_Event_Channel_Impl* ec_impl_;
};

#endif
