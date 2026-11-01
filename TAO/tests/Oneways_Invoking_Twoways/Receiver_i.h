// ===================================================================
/**
 *  @file   Receiver_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 **/
// ===================================================================

#ifndef TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_RECEIVER_I_H
#define TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_RECEIVER_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Receiver interface
/**
 * Skeleton implementation..
 */

class Receiver_i
  : public virtual POA_Test::Receiver
{
public:
  /// Constructor
  Receiver_i (CORBA::ORB_ptr orb,
              Test::Sender_ptr sender,
              CORBA::ULong iter);

  ///Destructor
  ~Receiver_i () override;

  Receiver_i (Receiver_i const &) = delete;
  Receiver_i (Receiver_i &&) = delete;
  Receiver_i &operator= (Receiver_i const &) = delete;
  Receiver_i &operator= (Receiver_i &&) = delete;

  // = The skeleton methods
  CORBA::Long receive_call () override;

  /// Retun the number of calls that every thread would make...
  CORBA::Long get_call_count () override;

  void shutdown () override;

  /// Number of calls processed so far..
  CORBA::ULong no_calls_;

private:
  /// Our Orb...
  CORBA::ORB_var orb_;

  /// THe sender..
  Test::Sender_var sender_;

  /// Number of invocations to be made on the sender..
  CORBA::ULong iteration_;
};

#include /**/ "ace/post.h"
#endif /*TAO_TESTS_ONEWAYS_INVOKING_TWOWAYS_RECEIVER_I_H*/
