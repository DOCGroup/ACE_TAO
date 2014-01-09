//
// $Id$
//

#ifndef LATENCYTEST_H
#define LATENCYTEST_H
#include /**/ "ace/pre.h"

#include "../BenchmarkS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::LatencyTest interface
class LatencyTest_Impl :
  public virtual POA_Benchmark::LatencyTest
{
public:
  /// Constructor
  LatencyTest_Impl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Long makeCall (CORBA::Long send_time);

  virtual void shutdown ();

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* LATENCYTEST_H */
