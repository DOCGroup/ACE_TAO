// -*- C++ -*-
//
// $Id$
#ifndef STUBFAULTCONSUMER_H
#define STUBFAULTCONSUMER_H
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/FT_NotifierC.h>

//////////////////////
// Forward references
class TAO_ORB_Manager;

/////////////////////
// Class declarations

/**
 * A stub fault consumer for use in unit testing Fault Notifier.
 */
class StubFaultConsumer 
  : public virtual POA_CosNotifyComm::StructuredPushConsumer 
{
public:
  /**
   * Default constructor.
   */
  StubFaultConsumer ();

  /**
   * virtual destructor.
   */
  virtual ~StubFaultConsumer ();

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Publish this objects IOR.
   */
  int self_register (
    TAO_ORB_Manager & orbManager,
    ::FT::FaultNotifier_var & notifier
    ACE_ENV_ARG_DECL);

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
  int self_unregister (ACE_ENV_SINGLE_ARG_DECL);


  size_t notifications() const;

public:

    ////////////////
    // CORBA methods
    virtual void push_structured_event(
      const CosNotification::StructuredEvent &notification
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      throw (CORBA::SystemException, CosEventComm::Disconnected);  

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      throw (CORBA::SystemException, CosNotifyComm::InvalidEventType);     

   virtual void disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw (CORBA::SystemException);           

  ////////////////////
  // Forbidden methods
private:
  StubFaultConsumer (const StubFaultConsumer & rhs);
  StubFaultConsumer & operator = (const StubFaultConsumer & rhs);

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
   * IOR of this object as assigned by orb.
   */
  CORBA::String_var ior_;

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

  size_t notifications_;

};


#endif /* STUBFAULTCONSUMER_H  */
