// This may look like C, but it's really -*- C++ -*-
// ===================================================================
/**
 *  @file   Sender_i.h
 *
 *  $Id$
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
  virtual ~Sender_i (void);

  // = The skeleton methods
  virtual void active_objects (CORBA::Short ao);

  virtual void send_ready_message (Test::Receiver_ptr receiver);

  virtual void ping (void);

  virtual void shutdown (void);

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
