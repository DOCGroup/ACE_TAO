
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_BIDIRECTIONAL_TEST_I_H
#define TAO_BIDIRECTIONAL_TEST_I_H

#include "testS.h"
#include "ace/Event_Handler.h"

/**
 * @class Callback_i
 *
 * @brief A callback object to the "client"
 *
 * To test that the server can call the client on the same
 * connection that was established by the client
 */
class Callback_i : public virtual POA_Callback
{
public:
  /// ctor
  Callback_i (CORBA::ORB_ptr orb, Simple_Server_ptr srv);
  Callback_i (Callback_i const &) = delete;
  Callback_i (Callback_i &&) = delete;
  Callback_i &operator= (Callback_i const &) = delete;
  Callback_i &operator= (Callback_i &&) = delete;

  /// Safe way to shutdown
  void shutdown () override;

  /// The callback method
  void callback_method () override;

private:
  /// The orb
  /// The server
  CORBA::ORB_var orb_;
  Simple_Server_var server_;
};

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public virtual POA_Simple_Server, public virtual ACE_Event_Handler
{
public:
  /// ctor
  Simple_Server_i (CORBA::ORB_ptr orb, int no_iterations);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Boolean do_callback) override;

  void callback_object (Callback_ptr callback) override;

  void shutdown () override;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act = nullptr) override;

private:
  /// The ORB
  CORBA::ORB_var orb_;

  /// Callback Object
  Callback_var callback_ {};

  /// Number of times the callback needs to be called
  int no_iterations_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BIDIRECTIONAL_TEST_I_H */
