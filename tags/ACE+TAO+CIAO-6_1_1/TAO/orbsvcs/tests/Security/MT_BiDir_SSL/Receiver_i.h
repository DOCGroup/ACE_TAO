//
// $Id$
//

#ifndef MT_BIDIR_RECEIVER_I_H
#define MT_BIDIR_RECEIVER_I_H
#include /**/ "ace/pre.h"

#include "ReceiverS.h"

/**
 * Simply print count how many bytes were received.
 */
class Receiver_i
  : public virtual POA_Receiver
{
public:
  /// Constructor
  Receiver_i (void);


  /// Get the number of events that have been received, local method
  CORBA::Long get_event_count (void);

  // = The skeleton methods
  virtual void receive_payload (const Receiver::Payload &payload);

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
};

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
