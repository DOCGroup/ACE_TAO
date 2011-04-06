
//=============================================================================
/**
 *  @file   simple_test_i.h
 *
 *  $Id$
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
  /// ctor
  Simple_Server_i (CORBA::ORB_ptr orb);

  // = The Simple_Server methods.
  CORBA::Boolean print_status (void);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "simple_test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_STRATEGIES_I_H */
