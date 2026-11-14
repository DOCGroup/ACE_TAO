//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================
#ifndef TAO_RETRY_ON_REPLY_FAILURE_I_H
#define TAO_RETRY_ON_REPLY_FAILURE_I_H

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
  Simple_Server_i (CORBA::ORB_ptr orb, int num_exceptions_to_throw,
                   int raise_exception);

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  CORBA::Boolean test_is_a (char const *type) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;

  int num_exceptions_to_throw_;
  int num_exceptions_thrown_;

  int raise_exception_;
};

#endif /* TAO_RETRY_ON_REPLY_FAILURE_I_H */
