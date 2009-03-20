// -*- C++ -*-
//
// $Id$
#ifndef STUBFAULTANALYZER_H
#define STUBFAULTANALYZER_H
#include /**/ <ace/pre.h>
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StubFaultConsumer.h"
#include "StubBatchConsumer.h"
#include <orbsvcs/FT_NotifierC.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>

#include "ace/Vector_T.h"
#include "ace/SString.h"

//////////////////////
// Forward references

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
   * Prepare to exit.
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (void);

  /////////////////
  // Implementation
private:
  int readIORFile(const ACE_TCHAR * fileName, CORBA::String_var & ior);

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
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_CString identity_;

  /**
   * a file to write to to signal "ready"
   */
  const ACE_TCHAR * readyFile_;

  /**
   * The detector's IOR  (-d option)
   */
  const ACE_TCHAR * detector_ior_;

  ::FT::FaultDetectorFactory_var factory_;

  /**
   * The notifier's IOR (-n option)
   */
  const ACE_TCHAR * notifier_ior_;

  ::FT::FaultNotifier_var notifier_;

  /**
   * A collection of files containing replica IORs
   */
  ACE_Vector < const char * > replicaIORs;

  ACE_Vector < FT::PullMonitorable_var > replicas_;


  StubFaultConsumer faultConsumer_;
  StubBatchConsumer batchConsumer_;

};

#include /**/ <ace/post.h>

#endif /* STUBFAULTANALYZER_H  */
