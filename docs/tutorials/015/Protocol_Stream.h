
// $Id$

#ifndef PROTOCOL_STREAM_H
#define PROTOCOL_STREAM_H

#include "ace/SOCK_Stream.h"
#include "ace/Stream.h"

typedef ACE_Stream<ACE_MT_SYNCH> Stream;

class ACE_Message_Block;
class Recv;
class Protocol_Task;

class Protocol_Stream
{
public:
    Protocol_Stream(void);
    ~Protocol_Stream(void);

        // Provide the stream with an ACE_SOCK_Stream on which it can
        // communicate.  If _reader is non-null, it will be added as
        // the reader task just below the stream head so that it can
        // process data read from the peer.
    int open( ACE_SOCK_Stream & _peer, Protocol_Task * _reader = 0 );

        // Close the stream.  All of the tasks & modules will also be closed.
    int close(void);

        // putting data onto the stream will pass it through all
        // protocol levels and send it to the peer.
    int put( ACE_Message_Block * & _message, ACE_Time_Value *
             _timeout = 0 );

        // get will cause the Recv task (at the tail of the stream) to
        // read some data from the peer and pass it upstream.  The
        // message block is then taken from the stream reader task's
        // message queue.
    int get( ACE_Message_Block * & _response, ACE_Time_Value *
             _timeout = 0 );

        // Tell the Recv task to read some data and send it upstream.
        // The data will pass through the protocol tasks and be queued 
        // into the stream head reader task's message queue.  If
        // you've installed a _reader in open() then that task's
        // recv() method will see the message and may consume it
        // instead of passing it to the stream head for queueing.
    int get(void);
    
    ACE_SOCK_Stream & peer(void)
        {
            return this->peer_;
        }
    
private:
    ACE_SOCK_Stream peer_;
    Stream stream_;

        // A task which is capable of receiving data on a socket.
        // Note that this is only useful by client-side applications.
    Recv * recv_;

    Stream & stream(void)
        {
            return this->stream_;
        }

        // Install the protocol tasks into the stream.
    int open(void);
};

#endif //  PROTOCOL_STREAM_H
