// $Id$

#ifndef RECV_H
#define RECV_h

#include "Protocol_Task.h"

class ACE_SOCK_Stream;

/* Get some data from the peer and send it upstream for
   de-protocol-ization.
*/
class Recv : public Protocol_Task
{
public:
  typedef Protocol_Task inherited;

  // Give it someone to talk to...
  Recv (ACE_SOCK_Stream &peer);
  ~Recv (void);

  // Trigger a read from the socket
  int get (void);

  // In some cases it might be easier to check the "state" of the Recv
  // object than to rely on return codes filtering back to you.
  int error (void)
  {
    return this->error_;
  }

protected:

  ACE_SOCK_Stream &peer (void)
  {
    return this->peer_;
  }

  // The baseclass will trigger this when our get() method is called.
  // A message block of the appropriate size is created, filled and
  // passed up the stream.
  int recv (ACE_Message_Block *message,
            ACE_Time_Value *timeout = 0);

private:
  // Our endpoint
  ACE_SOCK_Stream &peer_;

  // get() uses a bogus message block to cause the baseclass to invoke
  // recv().  To avoid memory thrashing, we create that bogus message
  // once and reuse it for the life of Recv.
  ACE_Message_Block *tickler_;

  // Our error flag (duh)
  int error_;
};

#endif /* RECV_H */
