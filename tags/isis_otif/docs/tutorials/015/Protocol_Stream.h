// $Id$

#ifndef PROTOCOL_STREAM_H
#define PROTOCOL_STREAM_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Stream.h"

// Shorthand for the stream.
typedef ACE_Stream<ACE_MT_SYNCH> Stream;

// Forward references to cut down on the number of #includes
class ACE_Message_Block;
class Recv;
class Protocol_Task;

/* The Protocol_Stream provides a tidy interface to an ACE_Stream
   setup to process a data block through a series of protocol stages.
*/
class Protocol_Stream
{
public:
  Protocol_Stream (void);
  ~Protocol_Stream (void);

  // Provide the stream with an ACE_SOCK_Stream on which it can
  // communicate.  If _reader is non-null, it will be added as the
  // reader task just below the stream head so that it can process
  // data read from the peer.
  int open (ACE_SOCK_Stream &peer,
            Protocol_Task *reader = 0);

  // Close the stream.  All of the tasks & modules will also be
  // closed.
  int close (void);

  // putting data onto the stream will pass it through all protocol
  // levels and send it to the peer.
  int put (ACE_Message_Block *&message,
           ACE_Time_Value *timeout = 0);

  // get will cause the Recv task (at the tail of the stream) to read
  // some data from the peer and pass it upstream.  The message block
  // is then taken from the stream reader task's message queue.
  int get (ACE_Message_Block *&response,
           ACE_Time_Value *timeout = 0);

  // Tell the Recv task to read some data and send it upstream.  The
  // data will pass through the protocol tasks and be queued into the
  // stream head reader task's message queue.  If you've installed a
  // _reader in open() then that task's recv() method will see the
  // message and may consume it instead of passing it to the stream
  // head for queueing.
  int get (void);

  ACE_SOCK_Stream &peer (void)
  {
    return this->peer_;
  }

private:
  // Our peer connection
  ACE_SOCK_Stream peer_;

  // The stream managing the various protocol tasks
  Stream stream_;

  // A task which is capable of receiving data on a socket.
  // Note that this is only useful by server-side applications.
  Recv *recv_;

  Stream &stream (void)
  {
    return this->stream_;
  }

  // Install the protocol tasks into the stream.
  int open (void);
};

#endif /* PROTOCOL_STREAM_H */
