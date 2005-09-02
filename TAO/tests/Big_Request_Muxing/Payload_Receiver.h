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
  , public virtual PortableServer::RefCountServantBase
{
public:
  Payload_Receiver (int expected);

  // = The skeleton methods
  virtual void more_data (const Test::Payload &payload
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  int count() const;
private:
  int message_count_;
  int expected_;
};

#include /**/ "ace/post.h"
#endif /* BIG_REQUEST_MUXING_PAYLOAD_RECEIVER_H */
