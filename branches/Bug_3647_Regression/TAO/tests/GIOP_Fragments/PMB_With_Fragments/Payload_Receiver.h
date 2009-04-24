//
// $Id$
//

#ifndef PMB_WITH_FRAGMENTS_PAYLOAD_RECEIVER_H
#define PMB_WITH_FRAGMENTS_PAYLOAD_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Payload_Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Payload_Receiver
  : public virtual POA_Test::Payload_Receiver
{
public:
  /// Constructor
  Payload_Receiver (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void more_data (const Test::Payload &payload);

  virtual void shutdown (void);

  int get_count (void) const;

public:
  int count_;
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* PMB_WITH_FRAGMENTS_PAYLOAD_RECEIVER_H */
