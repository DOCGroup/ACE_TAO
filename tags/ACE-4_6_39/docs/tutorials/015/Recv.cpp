
// $Id$

#include "Recv.h"
#include "ace/SOCK_Stream.h"

/* Construct the object with the peer reference and other appropriate
   initializations.
*/
Recv::Recv( ACE_SOCK_Stream & _peer )
        : Protocol_Task(), peer_(_peer), error_(0)
{
     // Create the tickler that get() will use to trigger recv()
     // through the baseclass.  Since we're single-threaded this is
     // probably overkill but it makes multi-threading easier if we
     // choose to do that.
    tickler_ = new ACE_Message_Block(1);
}

/* Be sure we manage the lifetime of the tickler to prevent a memory
   leak.
*/
Recv::~Recv(void)
{
    tickler_->release();
}

/* By putting the tickler to ourselves we cause things to happen in
   the baseclass that will invoke recv().  If we know we're single
   threaded we could directly call recv() and be done with it but then 
   we'd have to do something else if we're multi-threaded.  Just let
   the baseclass worry about those things!
*/
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
           end-of-string NULL character.  Since the OS layers (at least
           in Unix) will provide some buffering this isn't as bad as
           it may seem at first.

           The byte-at-a-time recv breaks horribly on Win32 where the
           WFMO_Reactor is used.  This is because the socket has been
           placed into non-blocking mode and only the recv() of the
           first byte will block.  The solution is to use
           ACE_Select_Reactor which doesn't change the socket
           characteristics.  We did that back in main(), so we should
           be in good shape now.
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

    int size = ACE_OS::atoi(msize);

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
