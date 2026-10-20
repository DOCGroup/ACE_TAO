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
  Simple_Server_i (CORBA::ORB_ptr orb, char const * key);
  // ctor

  Simple_Server_i ();
  // ctor

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  char * remote_call () override;

  char * shutdown (char const * key) override;

  char * abort (char const * key) override;

private:
  CORBA::ORB_var orb_;
  CORBA::String_var _key;
};

#endif /* BUG_2247_REGRESSION_TEST_I_H */
