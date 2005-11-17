//
// $Id$
//

#ifndef AMI_BUFFERING_H
#define AMI_BUFFERING_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::AMI_Buffering interface
class AMI_Buffering
  : public virtual POA_Test::AMI_Buffering
{
public:
  /// Constructor
  AMI_Buffering (CORBA::ORB_ptr orb,
                 Test::AMI_Buffering_Admin_ptr admin);

  // = The skeleton methods
  virtual void receive_data (const Test::Payload &the_payload
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void flush (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void sync (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Report request progress to this interface
  Test::AMI_Buffering_Admin_var admin_;
};

#include /**/ "ace/post.h"
#endif /* AMI_BUFFERING_H */
