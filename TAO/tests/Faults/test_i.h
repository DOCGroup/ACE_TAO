
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_FAILURE_TEST_I_H
#define TAO_FAILURE_TEST_I_H

#include "testS.h"

class Callback_i
  : public virtual POA_Callback
{
  // = TITLE
  //   A callback object to shutdown the "client"
  //
  // = DESCRIPTION
  //   To test that the server can tolerate client crashes the server
  //   receives a callback object.
  //
public:
  /// ctor
  explicit Callback_i (CORBA::ORB_ptr orb);
  Callback_i (Callback_i const &) = delete;
  Callback_i (Callback_i &&) = delete;
  Callback_i &operator= (Callback_i const &) = delete;
  Callback_i &operator= (Callback_i &&) = delete;

  /// The callback method
  void shutdown (CORBA::Boolean is_clean) override;

private:
  /// The orb
  CORBA::ORB_var orb_ {};
};

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
  CORBA::Long test_method (CORBA::Boolean do_callback,
                           CORBA::Boolean is_clean,
                           Callback_ptr callback) override;

  void shutdown_now (CORBA::Boolean is_clean) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_ {};
};

/**
 * @class Middle_i
 *
 * @brief Another implementation of the simple server...
 *
 * Implements the Simple_Server interface in test.idl
 */
class Middle_i : public virtual POA_Simple_Server
{
public:
  /// ctor
  Middle_i (CORBA::ORB_ptr orb,
            Simple_Server_ptr server);
  Middle_i (Middle_i const &) = delete;
  Middle_i (Middle_i &&) = delete;
  Middle_i &operator= (Middle_i const &) = delete;
  Middle_i &operator= (Middle_i &&) = delete;

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Boolean do_callback,
                           CORBA::Boolean is_clean,
                           Callback_ptr callback) override;

  void shutdown_now (CORBA::Boolean is_clean) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_ {};

  /// The real server
  Simple_Server_var server_ {};
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_FAILURE_TEST_I_H */
