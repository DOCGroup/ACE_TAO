
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Balachandran Natarajan
 */
//=============================================================================


#ifndef MPROFILE_FORWARDING_TEST_I_H
#define MPROFILE_FORWARDING_TEST_I_H

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
  Simple_Server_i (CORBA::ORB_ptr orb);

  // ctor

  /// ctor
  Simple_Server_i ();

  // = The Simple_Server methods.
  void remote_call ();

  void shutdown ();

 private:
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_CLIENT_TEST_I_H */
