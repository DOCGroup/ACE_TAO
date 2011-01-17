//
// $Id$
//

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

  // = The skeleton methods
  virtual CORBA::ULong request_count (void);

  virtual CORBA::ULong bytes_received_count (void);

  virtual void request_received (CORBA::ULong payload_length);

  virtual void flush (void);

  virtual void shutdown (void);

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
