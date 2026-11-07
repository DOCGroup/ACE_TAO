// -*- C++ -*-

//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_BIDIRECTIONAL_TEST_I_H
#define TAO_BIDIRECTIONAL_TEST_I_H

#include <memory>

#include "testS.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Guard_T.h"
#include "ace/Condition_T.h"

class Caller : public ACE_Task_Base
{
public:
  Caller (Callback_ptr cb, CORBA::ORB_ptr orb);
  Caller (Caller const &) = delete;
  Caller &operator= (Caller const &) = delete;
  Caller (Caller &&) = delete;
  Caller &operator= (Caller &&) = delete;
  void go ();

  int svc () override;

private:
  Callback_var const cbobj_;
  CORBA::ORB_var const orb_;
  TAO_SYNCH_MUTEX lock_;
  ACE_Condition<TAO_SYNCH_MUTEX> cond_;
  bool going_ {};
  CORBA::OctetSeq payload_;
};


/**
 * @class Callback_i
 *
 * @brief A callback object to the "client"
 *
 * To test that the server can call the client on the same
 * connection that was established by the client
 */
class Callback_i : public POA_Callback
{
public:
  /// ctor
  explicit Callback_i (CORBA::ORB_ptr orb);
  Callback_i (Callback_i const &) = delete;
  Callback_i &operator= (Callback_i const &) = delete;
  Callback_i (Callback_i &&) = delete;
  Callback_i &operator= (Callback_i &&) = delete;

  /// Safe way to shutdown
  void shutdown () override;

  /// The callback method
  void callback_method (CORBA::OctetSeq const &payload) override;

private:
  /// The orb
  CORBA::ORB_var const orb_;
};

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 * for (int loop = 0; loop < 10; loop++)
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  void ow_test () override;

  CORBA::Long test_method (CORBA::Boolean do_callback) override;

  void callback_object (Callback_ptr callback) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var const orb_;

  /// Callback Object
  std::unique_ptr<Caller> caller_ {};

  /// Number of times the callback needs to be called
  int ow_count_ {};
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_BIDIRECTIONAL_TEST_I_H */
