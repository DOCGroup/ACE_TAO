// $Id$

#ifndef BUG_2248_REGRESSION_TEST_I_H
#define BUG_2248_REGRESSION_TEST_I_H

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
  Simple_Server_i (CORBA::ORB_ptr orb, const char* key);
  // ctor

  Simple_Server_i (void);
  // ctor

  // = The Simple_Server methods.
  char* remote_call (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  char* shutdown (const char* key ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  char* abort (const char* key ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  CORBA::String_var _key;
};

#endif /* BUG_2248_REGRESSION_TEST_I_H */
