// ===================================================================
/**
 *  @file   Big_Reply_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 **/
// ===================================================================

#ifndef TAO_TESTS_BIG_REPLY_I_H
#define TAO_TESTS_BIG_REPLY_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Big_Reply interface
/**
 * Simply return a Big_Reply
 */

class Big_Reply_i
  : public virtual POA_Test::Big_Reply
{
public:
  /// Constructor
  Big_Reply_i (CORBA::ORB_ptr orb,
               CORBA::ULong length);
  Big_Reply_i (Big_Reply_i const &) = delete;
  Big_Reply_i (Big_Reply_i &&) = delete;
  Big_Reply_i &operator= (Big_Reply_i const &) = delete;
  Big_Reply_i &operator= (Big_Reply_i &&) = delete;

  ///Destructor
  ~Big_Reply_i () override = default;

  // = The skeleton methods
  Test::Octet_Seq *get_big_reply () override;

  void ping () override;

  void shutdown () override;

private:
  /// Our local ORB ptr
  CORBA::ORB_ptr orb_;

  /// Length of the reply sequence
  CORBA::ULong length_;

  /// The reply message
  Test::Octet_Seq_var reply_message_;
};

#include /**/ "ace/post.h"

#endif /*TAO_TESTS_BIG_REPLY_I_H*/
