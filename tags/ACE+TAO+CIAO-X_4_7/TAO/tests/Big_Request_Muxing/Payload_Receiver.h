//
// $Id$
//

#ifndef BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H
#define BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H
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
  Payload_Receiver (void);

  // = The skeleton methods
  virtual void more_data (const Test::Payload &payload
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Long get_message_count (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Count the number of messages received
  CORBA::ULong message_count_;

  /// Count the number of bytes received
  CORBA::ULong byte_count_;
};

#include /**/ "ace/post.h"
#endif /* BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H */
