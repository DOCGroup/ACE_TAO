
// $Id$

#include "Xmit.h"
#include "ace/SOCK_Stream.h"

/* Construct the object with the peer connection and choose not to
   activate ourselves into a dedicated thread.  You might get some
   performance gain by activating but if you really want a
   multi-threaded apprroach you should handle that as a separate
   issue.  Attempting to force threading at this level will likely
   cause more trouble than you want to deal with.
*/
Xmit::Xmit( ACE_SOCK_Stream & _peer )
        : Protocol_Task(), peer_(_peer)
{
}

Xmit::~Xmit(void)
{
}

/* Check to see if we're being closed by the stream (flags != 0) or if
   we're responding to the exit of our svc() method.
*/
int Xmit::close(u_long flags)
{
     // Take care of the baseclass closure.
    int rval = inherited::close(flags);

     // Only if we're being called at the stream shutdown do we close
     // the peer connection.  If, for some reason, we were activated
     // into one or more threads we wouldn't want to close the pipe
     // before all threads had a chance to flush their data.
    if( flags )
    {
        peer().close();
    }

    return( rval );
}

/* Our overload of send() will take care of sending the data to the
   peer.
*/
int Xmit::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    int rval;

    ACE_DEBUG ((LM_INFO, "(%P|%t) Xmit::send() sending (%s)(%d)\n", message->rd_ptr(), message->length() ));

     /* Since we're going to be sending data that may have been
        compressed and encrypted it's probably important for the
        receiver to get an entire "block" instead of having a
        partial read.

        For that reason, we'll send the length of the message block
        (in clear-text) to the peer so that it can then recv_n()
        the entire block contents in one read operation.
     */
    char msize[32];
    sprintf(msize,"%d",message->length());

     // Be sure we send the end-of-string NULL so that Recv will
     // know when to stop assembling the length.
    rval = this->peer().send_n( msize, strlen(msize)+1, 0, timeout );

    if( rval == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Xmit::send() Failed to send message size."), -1);
    }

     /* Now we send the actual data.  If you're worried about
        network efficiency then you may choose to create one buffer
        containing msize and the message data and send it all at
        once.
     */
    rval = this->peer().send_n( message->rd_ptr(), message->length(), 0, timeout );

     // Release the message block since we're done with it.
    message->release();

    return( rval );
}
