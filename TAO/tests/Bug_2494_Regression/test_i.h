#ifndef TAO_BUG_2494_REGRESSION_TEST_I_H
#define TAO_BUG_2494_REGRESSION_TEST_I_H

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
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  char *test_method (char const *x) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BUG_2494_REGRESSION_TEST_I_H */
