// ===================================================================
/**
 *  @file   Sender_i.h
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 **/
// ===================================================================

#ifndef TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_SENDER_I_H
#define TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_SENDER_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Sender interface
/**
 * Implements the oneway call send_ready_message. This in turn calls
 * the receiver with two way invocations..
 */

class Sender_i
  : public virtual POA_Test::Sender
{
public:
  /// Constructor
  explicit Sender_i (CORBA::ORB_ptr orb);

  ///Destructor
  ~Sender_i () override = default;
  Sender_i (Sender_i const &) = delete;
  Sender_i &operator= (Sender_i const &) = delete;
  Sender_i (Sender_i &&) = delete;
  Sender_i &operator= (Sender_i &&) = delete;

  // = The skeleton methods
  void active_objects (CORBA::ULong ao) override;

  void send_ready_message () override;

  CORBA::ULong get_number_received () const;

  CORBA::ULong get_active_objects () const;

private:
  /// Our local ORB ptr
  CORBA::ORB_var orb_;

  /// Number of active objects who are trying to connect
  CORBA::ULong active_objects_ {};

  /// Number of invocations received
  CORBA::ULong number_received_ {};
};

#include /**/ "ace/post.h"
#endif /*TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_SENDER_I_H*/
