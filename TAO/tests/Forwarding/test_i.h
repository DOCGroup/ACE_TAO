
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_FORWARDING_TEST_I_H
#define TAO_FORWARDING_TEST_I_H

#include "testS.h"

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  explicit Simple_Server_i (CORBA::ORB_ptr orb);

  // = The Simple_Server methods.
  CORBA::Boolean test_is_a (char const *type) override;

  void shutdown () override;

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

private:
  /// The ORB
  CORBA::ORB_var const orb_ {};
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_FORWARDING_TEST_I_H */
