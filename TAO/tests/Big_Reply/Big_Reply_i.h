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

  ///Destructor
  virtual ~Big_Reply_i ();

  // = The skeleton methods
  virtual Test::Octet_Seq *get_big_reply ();

  virtual void ping ();

  virtual void shutdown ();

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
