// $Id$

#ifndef TAO_AUTHORIZATION_TEST_I_H
#define TAO_AUTHORIZATION_TEST_I_H

#include "authorization_testS.h"

class Authorization_Server_i : public POA_Authorization_Server
{
  // = TITLE
  //   Authorization Server implementation
  //
  // = DESCRIPTION
  //   Implements the Authorization_Server interface.
  //   Has once method which helps find if the server is contacted.
  //
 public:

  /// Constructor
  Authorization_Server_i (CORBA::ORB_ptr orb);

  /// The Authorization Server method.
  /// Returns a boolean (true) if invoked.
  CORBA::Boolean check_access (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "authorization_test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_AUTHORIZATION_TEST_I_H */
