// -*- C++ -*-
//
// $Id$
#ifndef STUBBATCHCONSUMER_H
#define STUBBATCHCONSUMER_H
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/FT_NotifierC.h>

//////////////////////
// Forward references

/////////////////////
// Class declarations

/**
 * A stub fault consumer for use in unit testing Fault Notifier.
 */
class StubBatchConsumer
  : public virtual POA_CosNotifyComm::SequencePushConsumer
{
public:
  /**
   * Default constructor.
   */
  StubBatchConsumer ();

  /**
   * virtual destructor.
   */
  virtual ~StubBatchConsumer ();

  ::PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);
  ::PortableServer::ObjectId objectId()const;

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Publish this objects IOR.
   */
  int init (CORBA::ORB_var & orbManager, ::FT::FaultNotifier_var & notifier ACE_ENV_ARG_DECL);

  /**
   * Return a string to identify this object for logging/console message purposes.
   */
  const char * identity () const;

  /**
   * idle time activity.
   * @param result [out] status code to return from process
   * @returns 0 to continue; nonzero to quit
   */
  int idle(int &result);

  /**
   * Clean house for process shut down.
   */
  void fini (ACE_ENV_SINGLE_ARG_DECL);


public:

    ////////////////
    // CORBA methods

    virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , CosEventComm::Disconnected
      ));

    virtual void disconnect_sequence_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      throw (CORBA::SystemException, CosNotifyComm::InvalidEventType);

  ////////////////////
  // Forbidden methods
private:
  StubBatchConsumer (const StubBatchConsumer & rhs);
  StubBatchConsumer & operator = (const StubBatchConsumer & rhs);

  /////////////////////////
  // Implementation methods
private:

  ///////////////
  // Data Members
private:
  /**
   * The orb 
   */
  CORBA::ORB_var orb_;

  /**
   * The POA used to activate this object.
   */
  PortableServer::POA_var poa_;

  /**
   * The CORBA object id assigned to this object.
   */
  PortableServer::ObjectId_var objectId_;

  /**
   * A human-readable string to distinguish this from other FaultConsumers.
   */
  ACE_CString identity_;

  /**
   * boolean: request quit
   */
  int quit_;

  /**
   * the notifier
   */
  ::FT::FaultNotifier_var notifier_;

  /**
   * consumer id assigned by the notifier
   */
  ::FT::FaultNotifier::ConsumerId consumerId_;
};


#endif /* STUBBATCHCONSUMER_H  */
