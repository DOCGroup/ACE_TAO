
#ifndef SLEEP_SERVICE_H
#define SLEEP_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Sleep_Service interface
class Sleep_Service
  : public virtual POA_Test::Sleep_Service
{
public:
  /// Constructor
  explicit Sleep_Service (CORBA::ORB_ptr orb);
  ~Sleep_Service () override = default;

  // = The skeleton methods
  void go_to_sleep (CORBA::ULong microseconds) override;

  void shutdown () override;

  Sleep_Service (Sleep_Service const&) = delete;
  Sleep_Service (Sleep_Service&&) = delete;
  Sleep_Service& operator= (Sleep_Service const&) = delete;
  Sleep_Service& operator= (Sleep_Service&&) = delete;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_;
};

#include /**/ "ace/post.h"
#endif /* SLEEP_SERVICE_H */
