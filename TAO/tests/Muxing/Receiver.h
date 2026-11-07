
#ifndef MUXING_RECEIVER_H
#define MUXING_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Receiver
  : public POA_Test::Receiver
{
public:
  /// Constructor
  explicit Receiver (CORBA::ORB_ptr orb);
  Receiver (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver &operator= (Receiver &&) = delete;

  /// Print out the results
  void dump_results ();

  // = The skeleton methods
  void receive_data (Test::Payload const &payload) override;
  CORBA::Long get_event_count () override;

  void shutdown () override;

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_ {};
  CORBA::ULong byte_count_ {};
  /// Use an ORB reference to shutdown
  /// the application.
  CORBA::ORB_var orb_ {};
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
