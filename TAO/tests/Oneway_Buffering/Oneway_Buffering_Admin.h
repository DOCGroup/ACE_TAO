
#ifndef ONEWAY_BUFFERING_ADMIN_H
#define ONEWAY_BUFFERING_ADMIN_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Oneway_Buffering_Admin interface
class Oneway_Buffering_Admin
  : public virtual POA_Test::Oneway_Buffering_Admin
{
public:
  /// Constructor
  explicit Oneway_Buffering_Admin (CORBA::ORB_ptr orb);

  Oneway_Buffering_Admin (Oneway_Buffering_Admin const &) = delete;
  Oneway_Buffering_Admin &operator= (Oneway_Buffering_Admin const &) = delete;
  Oneway_Buffering_Admin (Oneway_Buffering_Admin &&) = delete;
  Oneway_Buffering_Admin &operator= (Oneway_Buffering_Admin &&) = delete;

  // = The skeleton methods
  CORBA::ULong request_count (CORBA::ULong expected_request_count) override;

  CORBA::ULong bytes_received_count (CORBA::ULong expected_bytes_received_count) override;

  void request_received (CORBA::ULong payload_length) override;

  void flush () override;

  void shutdown () override;

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Keep track of the number of requests received
  CORBA::ULong request_count_;

  /// Keep track of the number of requests received
  CORBA::ULong bytes_received_count_;
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_BUFFERING_H */
