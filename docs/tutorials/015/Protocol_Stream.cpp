
// $Id$

#include "Protocol_Stream.h"
#include "Protocol_Task.h"

#include "Xmit.h"
#include "Recv.h"

#include "Compressor.h"
#include "Crypt.h"

#define ENABLE_COMPRESSION
#define ENABLE_ENCRYPTION

typedef ACE_Module<ACE_MT_SYNCH> Module;
typedef ACE_Thru_Task<ACE_MT_SYNCH> Thru_Task;

Protocol_Stream::Protocol_Stream( void )
{
    ;
}

Protocol_Stream::~Protocol_Stream( void )
{
    ;
}

int Protocol_Stream::open( ACE_SOCK_Stream & _peer, Protocol_Task * _reader )
{
        // Initialize our peer() to read/write the socket we're given
    peer_.set_handle( _peer.get_handle() );

        // Construct (and remember) the Recv object so that we can
        // read from the peer()
    recv_ = new Recv( peer() );

        // Add the transmit and receive tasks to the head of the stream
    if( stream().push( new Module( "Xmit/Recv", new Xmit( peer() ), recv_ ) ) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "stream().push( xmit/recv )"), -1);
    }

        // Add any other protocol tasks to the stream.  Each one is
        // added at the head.  The net result is that Xmit/Recv are at 
        // the tail.
    if( this->open() == -1 )
    {
        return(-1);
    }

        // If a reader task was provided then push that in as the
        // upstream side of the next-to-head module.  Any data read
        // from the peer() will be sent through here last.  Server
        // applications will typically use this task to do the actual
        // processing of data.
    if( _reader )
    {
        if( stream().push( new Module( "Reader", new Thru_Task(), _reader ) ) == -1 )
        {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "stream().push( reader )"), -1);
        }
    }

    return(0);
}

int Protocol_Stream::open(void)
{
#if defined(ENABLE_COMPRESSION)
    if( stream().push( new Module( "compress", new Compressor(), new Compressor() ) ) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "stream().push( comprssor )"), -1);
    }
#endif // ENABLE_COMPRESSION
    
#if defined(ENABLE_ENCRYPTION)
    if( stream().push( new Module( "crypt", new Crypt(), new Crypt() ) ) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "stream().push( crypt )"), -1);
    }
#endif // ENABLE_ENCRYPTION
    return( 0 );
}

int Protocol_Stream::close(void)
{
    return stream().close();
}

// Simply pass the data directly to the ACE_Stream.
int Protocol_Stream::put(ACE_Message_Block * & _message, ACE_Time_Value * _timeout )
{
    return stream().put(_message,_timeout);
}

/* Tell the Recv module to read some data (eg -- get() from the peer
   and pass it upstream.
*/
int Protocol_Stream::get(void)
{
    if( ! recv_ )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) No Recv object!\n"), -1);
    }
        
    if( recv_->get() == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot queue read request\n"), -1);
    }

    if( recv_->error() )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Recv object error!\n"), -1);
    }

    return(0);
}

/* Take a message block off of the stream head reader's message
   queue.  If the queue is empty, use get() to read from the peer.
*/
int Protocol_Stream::get(ACE_Message_Block * & _response, ACE_Time_Value * _timeout )
{
    if( stream().head()->reader()->msg_queue()->is_empty() )
    {
        if( this->get() == -1 )
        {
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot get data into the stream.\n"), -1);
        }
    }
    
    return stream().head()->reader()->getq(_response,_timeout);
}
