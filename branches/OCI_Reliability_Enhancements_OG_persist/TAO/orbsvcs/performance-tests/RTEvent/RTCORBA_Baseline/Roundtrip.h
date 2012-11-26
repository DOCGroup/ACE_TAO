//
// $Id$
//

#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual POA_Test::Roundtrip
{
public:
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual Test::Timestamp test_method (Test::Timestamp send_time,
                                       CORBA::Long workload_in_usecs);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Translate the high resolution timer units into microseconds
  ACE_UINT32 gsf_;
};

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
