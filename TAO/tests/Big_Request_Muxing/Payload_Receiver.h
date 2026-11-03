
#ifndef BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H
#define BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#include <atomic>

/// Implement the Test::Payload_Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Payload_Receiver
  : public virtual POA_Test::Payload_Receiver
{
public:
  Payload_Receiver () = default;
  Payload_Receiver (Payload_Receiver const &) = delete;
  Payload_Receiver &operator= (Payload_Receiver const &) = delete;
  Payload_Receiver (Payload_Receiver &&) = delete;
  Payload_Receiver &operator= (Payload_Receiver &&) = delete;

  // = The skeleton methods
  void more_data (Test::Payload const &payload,
                  CORBA::Boolean maybe_lost) override;

  void ping () override;

  int count (bool maybe_lost = false) const;

private:
  std::atomic<int> message_count_ {0};
  std::atomic<int> maybe_lost_count_ {0};
};

#include /**/ "ace/post.h"
#endif /* BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H */
