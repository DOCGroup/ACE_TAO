
// $Id$

#include "Recv.h"
#include "ace/SOCK_Stream.h"

Recv::Recv( ACE_SOCK_Stream & _peer )
        : inherited(0), peer_(_peer), error_(0)
{
    tickler_ = new ACE_Message_Block(1);
}

Recv::~Recv(void)
{
    tickler_->release();
}

int Recv::get(void)
{
    return this->put( tickler_, 0 );
}

int Recv::recv(ACE_Message_Block * message, ACE_Time_Value *timeout)
{
    int rval;

        /* Xmit will send us the message length in clear-text.  I
           assume that will be less than 32-bytes!
        */
    char msize[32];
    int b = 0;

        /* Read from the socket one byte at a time until we see then
           end-of-string NULL character.
        */
    do
    {
        rval = this->peer().recv( &msize[b], 1, timeout );
        if( rval == -1 )
        {
            error_ = 1;
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Recv::recv() Failed to get message size."), -1);
        }
    }
    while( msize[b++] != 0 );

    int size = atoi(msize);

        // Make a block big enough to contain the data we'll read
    message = new ACE_Message_Block( size );

        // Read the actual message data into our new message block
    rval = this->peer().recv_n( message->wr_ptr(), size, 0, timeout );

        // If we got the data correctly then send it on upstream.
    if( rval > 0 )
    {
        message->wr_ptr( rval );
        return( this->put_next( message ) );
    }

        // Something bad happend on the recv_n().  Set an error flag
        // and return error.
    error_ = 1;
    
    return( -1 );
}
