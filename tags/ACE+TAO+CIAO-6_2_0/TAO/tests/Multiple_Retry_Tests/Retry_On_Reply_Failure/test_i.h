
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
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
  Simple_Server_i (CORBA::ORB_ptr orb, int num_exceptions_to_throw);

  // = The Simple_Server methods.
  CORBA::Boolean test_is_a (const char * type);

  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;

  int num_exceptions_to_throw_;
  int num_exceptions_thrown_;

  int raise_exception_;
};

#endif /* TAO_RETRY_ON_REPLY_FAILURE_I_H */
