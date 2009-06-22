// -*- C++ -*-
//
// $Id$
#ifndef STUBFAULTCONSUMER_H
#define STUBFAULTCONSUMER_H
#include /**/ <ace/pre.h>
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include <orbsvcs/FT_NotifierC.h>

#include "ace/SString.h"


//////////////////////
// Forward references

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


  ::PortableServer::POA_ptr _default_POA (void);
  ::PortableServer::ObjectId objectId()const;

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, ACE_TCHAR * argv[]);

  /**
   * Publish this objects IOR.
   */
  int init (CORBA::ORB_ptr orb, ::FT::FaultNotifier_var & notifier);

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
  int fini (void);


  size_t notifications() const;

public:

    ////////////////
    // CORBA methods
    virtual void push_structured_event(
      const CosNotification::StructuredEvent &notification
      );

   virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );

   virtual void disconnect_structured_push_consumer(void);

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
   * The POA used to activate this object.
   */
  PortableServer::POA_var poa_;

  /**
   * The CORBA object id assigned to this object.
   */
  PortableServer::ObjectId_var object_id_;


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
  ::FT::FaultNotifier::ConsumerId consumer_id_;

  size_t notifications_;

};

#include /**/ <ace/post.h>

#endif /* STUBFAULTCONSUMER_H  */
