// ===================================================================
/**
 *  @file   Sender_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
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
  Sender_i (CORBA::ORB_ptr orb,
            CORBA::ULong no);

  ///Destructor
  ~Sender_i () override;

  Sender_i (Sender_i const &) = delete;
  Sender_i (Sender_i &&) = delete;
  Sender_i &operator= (Sender_i const &) = delete;
  Sender_i &operator= (Sender_i &&) = delete;

  // = The skeleton methods
  void active_objects (CORBA::Short ao) override;

  void send_ready_message (Test::Receiver_ptr receiver) override;

  void ping () override;

  void shutdown () override;

private:
  /// Our local ORB ptr
  CORBA::ORB_var orb_;

  /// Number of invocations to be made on the receiver
  CORBA::ULong number_;

  /// Number of active objects who are trying to connect
  CORBA::Short active_objects_;
};

#include /**/ "ace/post.h"
#endif /*TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_SENDER_I_H*/
