#ifndef TAO_BUG_2494_REGRESSION_TEST_I_H
#define TAO_BUG_2494_REGRESSION_TEST_I_H

#include "testS.h"

/// Implements the `Simple_Server` interface from `test.idl`.
class Simple_Server_i : public POA_Simple_Server
{
public:
  explicit Simple_Server_i (CORBA::ORB_ptr orb);

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  char *test_method (char const *x) override;

  void shutdown () override;

private:
  /// The ORB used by this servant.
  CORBA::ORB_var const orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BUG_2494_REGRESSION_TEST_I_H */
