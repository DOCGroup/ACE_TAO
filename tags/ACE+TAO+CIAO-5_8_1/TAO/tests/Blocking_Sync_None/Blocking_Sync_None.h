//
// $Id$
//

#ifndef BLOCKING_SYNC_NONE_H
#define BLOCKING_SYNC_NONE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Blocking_Sync_None interface
class Blocking_Sync_None
  : public virtual POA_Test::Blocking_Sync_None
{
public:
  /// Constructor
  Blocking_Sync_None (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void slow_operation (const Test::Payload &the_payload,
                               CORBA::ULong sleep_microseconds);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BLOCKING_SYNC_NONE_H */
