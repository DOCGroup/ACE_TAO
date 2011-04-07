// -*- C++ -*-
//
// $Id$
#ifndef STUBFAULTNOTIFIER_H_
#define STUBFAULTNOTIFIER_H_

#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include  /**/ <ace/pre.h>

#include <orbsvcs/FT_NotifierS.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>
#include <ace/Vector_T.h>
#include <ace/SString.h>

//////////////////////
// Forward references

/////////////////////
// Class declarations

/**
 * A stub implementation of the FaultNotifier interface
 */
class  StubFaultNotifier : public virtual POA_FT::FaultNotifier
{
  //////////////////////
  // non-CORBA interface
public:
  /**
   * Default constructor.
   */
  StubFaultNotifier ();

  /**
   * Virtual destructor.
   */
  virtual ~StubFaultNotifier ();

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, ACE_TCHAR * argv[]);

  /**
   * Initialize.
   */
  int init (CORBA::ORB_ptr orb);

  /**
   * Prepare to exit.
   */
  int fini (void);

  /**
   * Return a string to identify this object for logging/console message purposes.
   */
  const ACE_TCHAR * identity () const;

  /**
   * idle time activity.
   * @param result [out] status code to return from process
   * @returns 0 to continue; nonzero to quit
   */
  int idle(int &result);

  // override virtuals
::PortableServer::POA_ptr _default_POA (void);

  PortableServer::ObjectId objectId()const;


  /**
   * Clean house for process shut down.
   */
  void shutdown_i (void);

  ////////////////////////////////
  // CORBA interface FaultNotifier


  virtual void push_structured_fault (
      const CosNotification::StructuredEvent & event
    );

  virtual void push_sequence_fault (
    const CosNotification::EventBatch & events
  );

  virtual ::CosNotifyFilter::Filter_ptr create_subscription_filter (
    const char * constraint_grammar
  );

  virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  );

  virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  );

  virtual void disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
  );

  //////////////////////////////////////////
  // CORBA interface PullMonitorable methods
  virtual CORBA::Boolean is_alive (void);

  /////////////////
  // Implementation
private:
  int write_ior_file();

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
   * IOR of this object as assigned by poa
   */
  CORBA::String_var ior_;

  /**
   * A file to which the factory's IOR should be written.
   */
  const ACE_TCHAR * ior_output_file_;

  /**
   * A file from which the detector's IOR should be read.
   */
  const ACE_TCHAR * detector_ior_;

  ::FT::FaultDetectorFactory_var factory_;

  /**
   * A collection of files containing replica IORs
   */
  ACE_Vector < const char * > iorReplicaFiles_;


  ACE_Vector < FT::PullMonitorable_var > replicas_;
  /**
   * A name to be used to register with the name service.
   */
  ACE_CString ns_name_;

  /**
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_TString identity_;
};

#include /**/ <ace/post.h>

#endif /* STUBFAULTNOTIFIER_H_  */
