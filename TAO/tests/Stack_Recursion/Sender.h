//
// $Id$
//

#ifndef STACK_RECURSION_SENDER_H
#define STACK_RECURSION_SENDER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Sender
  : public virtual POA_Test::Sender
{
public:
  /// Constructor
  Sender (CORBA::ORB_ptr orb);

  /// Print out the results
  void dump_results (void);

  /// Get the status of the flag..
  bool is_done (void) const;

  // = The skeleton methods
  virtual CORBA::Boolean get_data (CORBA::ULong size,
                                   Test::Payload_out payload);

  virtual CORBA::Long get_event_count (void);

  virtual void ping (void);

  virtual void shutdown (void);

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
  CORBA::ORB_var orb_;

  bool is_done_;
};

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
