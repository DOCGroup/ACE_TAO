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
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommS.h"
#include "orbsvcs/FT_NotifierC.h"
#include "FT_ReplicationManagerLib_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  ///////////////////////
  // Forward declarations
  class FT_FaultAnalyzer;

  /**
  * Implement the CosNotifyComm::StructuredPushConsumer interface.
  *
  */
  class TAO_ReplicationManagerLib_Export FT_FaultConsumer
    : public virtual POA_CosNotifyComm::StructuredPushConsumer
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
      TAO::FT_FaultAnalyzer * fault_analyzer
      ACE_ENV_ARG_DECL);

    /**
    * Clean house for process shut down.
    * - Disconnect from FT::FaultNotifier.
    * - Deactivate from the POA.
    */
    int fini (ACE_ENV_SINGLE_ARG_DECL);

    /**
    * Accessor for a duplicate of this consumer's object reference.
    */
    CosNotifyComm::StructuredPushConsumer_ptr consumer_ref ();

    //@@ For testing purposes only, will be removed later.
    /**
    * Accessor for the number of notifications we have received.
    */
    size_t notifications () const;

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
    FT_FaultConsumer (const FT_FaultConsumer & rhs);
    /// Assignment operator.
    FT_FaultConsumer & operator = (const FT_FaultConsumer & rhs);

    ///////////////
    // Data Members
  private:

    /// The POA with which we are activated.
    PortableServer::POA_var poa_;

    /// The ObjectId from our activation in the POA.
    PortableServer::ObjectId_var object_id_;

    /// The FaultNotifier's object reference.
    FT::FaultNotifier_var fault_notifier_;

    /// Application-specific Fault Analyzer.
    TAO::FT_FaultAnalyzer * fault_analyzer_;

    /// ConsumerId assigned by the notifier.
    FT::FaultNotifier::ConsumerId consumer_id_;

    /// Our consumer object reference.
    CosNotifyComm::StructuredPushConsumer_var consumer_ref_;

    ///TODO: Remove this later, it is just for testing.
    // Keep track of how many notifications we have received.
    size_t notifications_;

  };

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_FAULT_CONSUMER_H_ */

