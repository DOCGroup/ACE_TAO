
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
  explicit Sender (CORBA::ORB_ptr orb);
  Sender (Sender const &) = delete;
  Sender (Sender &&) = delete;
  Sender &operator= (Sender const &) = delete;
  Sender &operator= (Sender &&) = delete;

  /// Print out the results
  void dump_results ();

  /// Get the status of the flag..
  bool is_done () const;

  // = The skeleton methods
  CORBA::Boolean get_data (CORBA::ULong size,
                           Test::Payload_out payload) override;

  CORBA::Long get_event_count () override;

  void ping () override;

  void shutdown () override;

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
  CORBA::ORB_var orb_;

  bool is_done_;
};

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
