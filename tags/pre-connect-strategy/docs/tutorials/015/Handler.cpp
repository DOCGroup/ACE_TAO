
// $Id$

#include "Handler.h"
#include "Protocol_Task.h"

/* The Protocol_Stream gives us the option to insert a Protocol_Task
   to process data received by the stream.  We'll get into the details 
   more when we talk about the stream in detail.  For now it's enough
   to know that Handler_Task::recv() will be invoked by the stream
   after data from the client has been received and processed (eg --
   decrypted, uncompressed, and whatever else the protocol requires.)
*/
class Handler_Task : public Protocol_Task
{
public:

        // Typical...
    typedef Protocol_Task inherited;

        // Simple...
    Handler_Task(void);
    ~Handler_Task(void);

protected:

        // recv() is invoked after received data has been fully
        // processed by the protocol rules.  Data processing typically 
        // done in handle_input() can then be done here.
     int recv(ACE_Message_Block * message,
              ACE_Time_Value *timeout = 0);
};

Handler::Handler(void)
{
    ;
}

Handler::~Handler(void)
{
    ;
}

/* The Acceptor will open() us once the peer() connection is
   established.  There are a couple of things we have to do here
   before we're ready to receive data from the client.
*/
int Handler::open (void *)
{
    ACE_INET_Addr addr;

        // Make sure that we can get the peer's address.  If we can't
        // then there may be a network error or something else that
        // will prevent communicating with the client.  This is
        // something you'll want to do in every event handler you create.
    if (this->peer ().get_remote_addr (addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot get remote addr\n"), -1);

        // Announce the client
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected with %s\n", addr.get_host_name() ));

        // Here's the first new twist to the old event handler.
        // Before we can use the Protocol_Stream to communicate with
        // the peer, we must open() it.  We provide the stream with
        // the peer() so that it will have a valid socket on which to
        // read client requests and send our responses.  We also
        // provide a Handler_Task instance that will ultimately be
        // responsible for processing any client data we receive.
    int rval = stream().open( this->peer(), new Handler_Task() );

        // Of course, we have to account for the chance that the
        // stream's open() may fail.
    if( rval == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot open the protocol stream.\n"), -1);
    }
    
        // Now that we know the client is valid and that the stream is 
        // ready for business we can register with the gloabl reactor
        // instance.  Here again is an opportunity for improvement if
        // we expect to have mulitple Server object instances.
    if (ACE_Reactor::instance()->register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot register with reactor\n"), -1);

    return rval;
}

/* This is a fairly typical destroy() method that can be shared by
   both close() and handle_close().
*/
void Handler::destroy (void)
{
    ACE_Reactor::instance()->remove_handler(this,ACE_Event_Handler::READ_MASK|ACE_Event_Handler::DONT_CALL);

    this->peer ().close ();

    delete this;
}

/* In this simple application we just forward the close() and
   handle_close() requests right on to the destroy() method.
*/

int Handler::close (u_long)
{
    this->destroy ();
    return 0;
}

int Handler::handle_close(ACE_HANDLE, ACE_Reactor_Mask _mask)
{
    this->destroy();
    return 0;
}

/* Unlike a "traditional" handle_input() ours is very simple.  Because 
   of the use of the protocol stream, we delegate the read function to 
   the stream's get() and rely on our Handler_Task to do the real work.
*/
int Handler::handle_input (ACE_HANDLE)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Activity from client\n" ));

        // This will cause a blocking read from the peer().  The data
        // will then be pushed through the protocol stream.
    if( stream().get( ) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Cannot get data from protocol stream\n"), -1);
    }
    
    return 0;
}

/* A Protocol_Task is derived from ACE_Task and has the option of
   running in one or more threads.  I've chosen here to construct the
   baseclass with no threads but it should work just fine with one or
   more if you need.  Unless you're sharing the Handler_Task with
   several peers, however, you're probably just wasting a thread to
   activate it.  On the other hand, if your reactor is running in a
   single thread (as in this example) then you can easily implement
   thread-per-connection concurrency by giving the baseclass one thread.
*/
Handler_Task::Handler_Task(void)
        : inherited()
{
    ;
}

Handler_Task::~Handler_Task(void)
{
    ;
}

/* When installed into the protocol stream, the Handler_Task's recv()
   method will be called when data is ready for processing.
 */
int Handler_Task::recv(ACE_Message_Block * message,
                       ACE_Time_Value *timeout )
{
        // Announce the request we got from the client
    ACE_DEBUG ((LM_INFO, "(%P|%t) Handler_Task::recv() got (%s)\n", message->rd_ptr() ));

        // Create a response message to send to the client
    ACE_Message_Block * response = new ACE_Message_Block( 128 );

        // Nothing very original about this I'm afraid...
    ACE_OS::sprintf( response->wr_ptr(), "You Said:  (%s)", message->rd_ptr() );
    response->wr_ptr( strlen(response->wr_ptr())+1 );

        // Release the original message block now that we're through
        // "processing" it.
    message->release();

        // Turn the message around and send it back down the Stream.
        // In other words, we invoke the put() method on the
        // Protocol_Stream without having to have a direct reference
        // to the stream object.
    return this->reply( response, timeout );
}
