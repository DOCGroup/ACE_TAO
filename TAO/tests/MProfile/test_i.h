
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_MPROFILE_TEST_I_H
#define TAO_MPROFILE_TEST_I_H

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
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  CORBA::Long remote_call () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  CORBA::Long val_ {1};
};

#endif /* TAO_MPROFILE_TEST_I_H */
