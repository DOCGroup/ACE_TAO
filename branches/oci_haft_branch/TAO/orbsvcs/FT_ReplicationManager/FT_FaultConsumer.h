/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultConsumer.h
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================


#ifndef FT_FAULT_CONSUMER_H_
#define FT_FAULT_CONSUMER_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommS.h"
#include "orbsvcs/FT_NotifierC.h"
#include "orbsvcs/FT_ReplicationManagerC.h"

namespace TAO
{
  /**
  * Implement the CosNotifyComm::StructuredPushConsumer interface.
  *
  */
  class FT_FaultConsumer
    : public virtual POA_CosNotifyComm::StructuredPushConsumer
    , public virtual PortableServer::RefCountServantBase
  {

    //////////////////////
    // non-CORBA interface

  public:
    /**
    * Default constructor.
    */
    FT_FaultConsumer ();

    /**
    * Destructor.
    */
    virtual ~FT_FaultConsumer ();

    /**
    * Connect to the FT::FaultNotifier.
    * Note: We make the following assumptions about what the
    * application will do:
    * - Create an instance of this consumer class.
    * - Obtain the object reference of the FaultNotifier to which this
    *   consumer should connect.
    * - Call this init() method, passing it the POA with which we
    *   have been activated, the FaultNotifier, and ReplicationManager
    *   object references.
    */
    int init (
      PortableServer::POA_ptr poa,
      FT::FaultNotifier_ptr fault_notifier,
      FT::ReplicationManager_ptr replication_manager
      ACE_ENV_ARG_DECL);

    /**
    * Clean house for process shut down.
    * - Disconnect from FT::FaultNotifier.
    * - Deactivate from the POA.
    */
    int fini (ACE_ENV_SINGLE_ARG_DECL);

  public:

    /////////////////////////
    // Implementation methods
  private:

    /**
    * Extract the value of the MinimumNumberReplicas property from
    * the_criteria.
    */
    int get_minimum_number_replicas (
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      CORBA::UShort & minimum_number_replicas) const;

  public:

    /**
    * @name POA_CosNotifyComm::StructuredPushConsumer Methods
    *
    * Methods required by the POA_CosNotifyComm::StructuredPushConsumer 
    * interface.
    */
    //@{

    ////////////////
    // CORBA methods
    virtual void push_structured_event (
      const CosNotification::StructuredEvent &notification
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected));

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyComm::InvalidEventType));

   virtual void disconnect_structured_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((CORBA::SystemException));

    //@}

    ////////////////////
    // Forbidden methods
  private:
    /// Copy constructor.
    FT_FaultConsumer (
        const FT_FaultConsumer & rhs);
    /// Assignment operator.
    FT_FaultConsumer & operator = (
        const FT_FaultConsumer & rhs);

    ///////////////
    // Data Members
  private:

    /// The POA with which we are activated.
    PortableServer::POA_var poa_;

    /// The FaultNotifier's object reference.
    FT::FaultNotifier_var fault_notifier_;

    /// The ReplicationManager's object reference.
    FT::ReplicationManager_var replication_manager_;

    /// ConsumerId assigned by the notifier.
    FT::FaultNotifier::ConsumerId consumer_id_;

    ///TODO: Remove this later, it is just for testing.
    // Keep track of how many notifications we have received.
    size_t notifications_;

  };

} // namespace TAO

#include "ace/post.h"

#endif  /* FT_FAULT_CONSUMER_H_ */

