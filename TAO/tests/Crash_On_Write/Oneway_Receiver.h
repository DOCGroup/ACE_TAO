
#ifndef ONEWAY_RECEIVER_H
#define ONEWAY_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Oneway_Receiver interface
class Oneway_Receiver
  : public virtual POA_Test::Oneway_Receiver
{
public:
  /// Constructor
  Oneway_Receiver () = default;

  Oneway_Receiver (Oneway_Receiver const &) = delete;
  Oneway_Receiver (Oneway_Receiver &&) = delete;
  Oneway_Receiver & operator= (Oneway_Receiver const &) = delete;
  Oneway_Receiver & operator= (Oneway_Receiver &&) = delete;

  // = The skeleton methods
  void receive_oneway () override;
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_RECEIVER_H */
