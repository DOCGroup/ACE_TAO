
#ifndef AMI_BUFFERING_ADMIN_H
#define AMI_BUFFERING_ADMIN_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::AMI_Buffering_Admin interface
class AMI_Buffering_Admin
  : public virtual POA_Test::AMI_Buffering_Admin
{
public:
  /// Constructor
  AMI_Buffering_Admin (CORBA::ORB_ptr orb);
  AMI_Buffering_Admin (AMI_Buffering_Admin const &) = delete;
  AMI_Buffering_Admin &operator= (AMI_Buffering_Admin const &) = delete;
  AMI_Buffering_Admin (AMI_Buffering_Admin &&) = delete;
  AMI_Buffering_Admin &operator= (AMI_Buffering_Admin &&) = delete;

  // = The skeleton methods
  CORBA::ULong request_count () override;

  CORBA::ULong bytes_received_count () override;

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
#endif /* AMI_BUFFERING_H */
