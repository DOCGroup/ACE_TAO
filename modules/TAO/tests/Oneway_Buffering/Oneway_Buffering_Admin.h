//
// $Id$
//

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
  Oneway_Buffering_Admin (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::ULong request_count (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong bytes_received_count (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void request_received (CORBA::ULong payload_length)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void flush (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
