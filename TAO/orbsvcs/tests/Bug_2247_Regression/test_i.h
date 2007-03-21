// $Id$

#ifndef BUG_2247_REGRESSION_TEST_I_H
#define BUG_2247_REGRESSION_TEST_I_H

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
  char* remote_call (void);

  char* shutdown (const char* key);

  char* abort (const char* key);

private:
  CORBA::ORB_var orb_;
  CORBA::String_var _key;
};

#endif /* BUG_2247_REGRESSION_TEST_I_H */
