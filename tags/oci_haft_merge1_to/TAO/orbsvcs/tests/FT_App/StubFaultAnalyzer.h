// -*- C++ -*-
//
// $Id$
#ifndef STUBFAULTANALYZER_H
#define STUBFAULTANALYZER_H
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StubFaultConsumer.h"
#include "StubBatchConsumer.h"
#include <orbsvcs/FT_NotifierC.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>

#include "ace/Vector_T.h"

//////////////////////
// Forward references
class TAO_ORB_Manager;

/////////////////////
// Class declarations

/**
 * A stub fault analyzer for use in unit testing.
 */
class  StubFaultAnalyzer
{
public:
  /**
   * Default constructor.
   */
  StubFaultAnalyzer ();

  /**
   * destructor.
   */
  ~StubFaultAnalyzer ();

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Publish this objects IOR.
   */
  int self_register (TAO_ORB_Manager & orbManager);

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
   * Revoke the publication of this objects IOR.
   * @return zero for success; nonzero is process return code for failure.
   */
  int self_unregister (ACE_ENV_SINGLE_ARG_DECL);

  /////////////////
  // Implementation
private:
  int readIORFile(const char * fileName, CORBA::String_var & ior);

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
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_CString identity_;

  /**
   * a file to write to to signal "ready"
   */
  const char * readyFile_;

  /**
   * A file from which the detecor's IOR should be read.
   */
  const char * iorDetectorFile_;

  ::FT::FaultDetectorFactory_var factory_;

  /**
   * A file from which the notifier's IOR should be read.
   */
  const char * iorNotifierFile_;

  ::FT::FaultNotifier_var notifier_;

  /**
   * A collection of files containing replica IORs
   */
  ACE_Vector < const char * > iorReplicaFiles_;
  char * replicaIorBuffer_;

  ACE_Vector < FT::PullMonitorable_var > replicas_;


  StubFaultConsumer faultConsumer_;
  StubBatchConsumer batchConsumer_;

};


#endif /* STUBFAULTANALYZER_H  */
