
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
  explicit Blocking_Sync_None (CORBA::ORB_ptr orb);
  Blocking_Sync_None (Blocking_Sync_None const &) = delete;
  Blocking_Sync_None (Blocking_Sync_None &&) = delete;
  Blocking_Sync_None &operator= (Blocking_Sync_None const &) = delete;
  Blocking_Sync_None &operator= (Blocking_Sync_None &&) = delete;

  // = The skeleton methods
  void slow_operation (Test::Payload const &the_payload,
                       CORBA::ULong sleep_microseconds) override;

  void shutdown () override;

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BLOCKING_SYNC_NONE_H */
