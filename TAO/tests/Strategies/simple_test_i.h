
//=============================================================================
/**
 *  @file   simple_test_i.h
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#ifndef TAO_STRATEGIES_TEST_I_H
#define TAO_STRATEGIES_TEST_I_H

#include "simple_testS.h"

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
  explicit Simple_Server_i (CORBA::ORB_ptr orb);

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  CORBA::Boolean print_status () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_ {};
};

#if defined(__ACE_INLINE__)
#include "simple_test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_STRATEGIES_I_H */
