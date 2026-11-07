
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_IOGR_TEST_I_H
#define TAO_FT_IOGR_TEST_I_H

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
  Simple_Server_i (Simple_Server_i const&) = delete;
  Simple_Server_i (Simple_Server_i&&) = delete;
  Simple_Server_i& operator= (Simple_Server_i const&) = delete;
  Simple_Server_i& operator= (Simple_Server_i&&) = delete;

  // = The Simple_Server methods.
  void remote_call () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_ {};
};

#endif /* TAO_FT_IOGR_TEST_I_H */
