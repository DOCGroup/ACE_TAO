// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/MProfile_Connection_Timeout
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Balachandran Natarajan <bala@isis-server.isis.vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_MPROFILE_CONNECTION_TIMEOUT_TEST_I_H
#define TAO_MPROFILE_CONNECTION_TIMEOUT_TEST_I_H

#include "testS.h"

class Simple_Server_i : public POA_Simple_Server
{
  // = TITLE
  //   Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the Simple_Server interface in test.idl
  //
public:
  Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  Simple_Server_i (void);
  // ctor

  // = The Simple_Server methods.
  CORBA::ULongLong remote_call (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  CORBA::Long val_;

};

#endif /* TAO_MPROFILE_CONNECTION_TIMEOUT_TEST_I_H */
