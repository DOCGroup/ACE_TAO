//
// $Id$
//

#ifndef LATENCYTEST_H
#define LATENCYTEST_H
#include /**/ "ace/pre.h"

#include "BenchmarkS.h"

/// Implement the Test::LatencyTest interface
class LatencyTest
: public virtual POA_Benchmark::LatencyTest
{
 public:
  /// Constructor
  LatencyTest (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Long makeCall (CORBA::Long send_time);

  virtual void shutdown ();

 private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* LATENCYTEST_H */
