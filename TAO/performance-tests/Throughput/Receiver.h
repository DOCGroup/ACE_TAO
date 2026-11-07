
#ifndef THROUGHPUT_RECEIVER_H
#define THROUGHPUT_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/OS_NS_time.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver interface
class Receiver
  : public virtual POA_Test::Receiver
{
public:
  /// Constructor
  Receiver () = default;
  Receiver (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver &operator= (Receiver &&) = delete;

  // = The skeleton methods
  void receive_data (Test::Message const &message) override;

  void done () override;

private:
  /// The timestamp for the first message
  ACE_hrtime_t start_time_ {};

  /// The number of messages received
  size_t message_count_ {};

  /// The number of bytes received
  size_t byte_count_ {};

  /// The timestamp for the last message
  ACE_hrtime_t last_message_time_ {};

  /// The id for the last message
  CORBA::ULong last_message_id_ {};
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* THROUGHPUT_RECEIVER_H */
