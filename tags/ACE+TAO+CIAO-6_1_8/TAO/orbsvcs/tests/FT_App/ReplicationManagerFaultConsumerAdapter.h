// -*- C++ -*-
//
// $Id$
#ifndef REPLICATION_MANAGER_FAULT_CONSUMER_ADAPTER_H
#define REPLICATION_MANAGER_FAULT_CONSUMER_ADAPTER_H
#include /**/ <ace/pre.h>
#include <ace/ACE.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/FT_ReplicationManager/FT_FaultConsumer.h>
#include <orbsvcs/FT_NotifierC.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>
#include <ace/Vector_T.h>
#include <ace/SString.h>

/////////////////////
// Class declarations

/**
 * An adapter to the Replication Manager's fault consumer
 * for use in unit testing Fault Notifier and Fault Consumer.
 */
class ReplicationManagerFaultConsumerAdapter
{
public:
  /**
   * Default constructor.
   */
  ReplicationManagerFaultConsumerAdapter ();

  /**
   * virtual destructor.
   */
  virtual ~ReplicationManagerFaultConsumerAdapter ();

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, ACE_TCHAR * argv[]);

  /**
   * Initialize this object
   */
  int init (CORBA::ORB_ptr orb);

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

  /////////////////
  // Implementation
private:

  ////////////////////
  // Forbidden methods
private:
  ReplicationManagerFaultConsumerAdapter (
    const ReplicationManagerFaultConsumerAdapter & rhs);
  ReplicationManagerFaultConsumerAdapter & operator =
    (const ReplicationManagerFaultConsumerAdapter & rhs);

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
   * A human-readable string to distinguish this from other FaultConsumers.
   */
  ACE_CString identity_;

  /**
   * boolean: request quit
   */
  int quit_;

  /**
   * a file to write to to signal "ready"
   */
  const ACE_TCHAR * readyFile_;

  /**
   * Detector's IOR
   */
  const ACE_TCHAR * detector_ior_;

  /**
   * The FaultDetectorFactory object reference.
   */
  ::FT::FaultDetectorFactory_var factory_;

  /**
   * A collection of replica IORs
   */
  ACE_Vector < const char * > replica_iors_;

  /**
   * A collection of replica references.
   */
  ACE_Vector < FT::PullMonitorable_var > replicas_;

  /**
   * A file from which the notifier's IOR should be read.
   */
  const ACE_TCHAR * notifier_ior_;

  /**
   * the fault notifier
   */
  ::FT::FaultNotifier_var notifier_;

  /**
   * consumer id assigned by the notifier
   */
  ::FT::FaultNotifier::ConsumerId consumerId_;

  /**
   * the real fault consumer
   */
  TAO::FT_FaultConsumer* p_fault_consumer_;
  PortableServer::ServantBase_var consumer_servant_;

};
#include /**/ <ace/post.h>

#endif /* REPLICATION_MANAGER_FAULT_CONSUMER_ADAPTER_H  */
