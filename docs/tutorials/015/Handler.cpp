
// $Id$

#include "Handler.h"
#include "Protocol_Task.h"

class Handler_Task : public Protocol_Task
{
public:

    typedef Protocol_Task inherited;
    
    Handler_Task(void);
    ~Handler_Task(void);

protected:
    
     int recv(ACE_Message_Block * message,
              ACE_Time_Value *timeout = 0);
};

Handler_Task::Handler_Task(void)
        : inherited(0)
{
    ;
}

Handler_Task::~Handler_Task(void)
{
    ;
}

int Handler_Task::recv(ACE_Message_Block * message,
                       ACE_Time_Value *timeout )
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Handler_Task::handle_input() got (%s)\n", message->rd_ptr() ));

    ACE_Message_Block * response = new ACE_Message_Block( 128 );

    ACE_OS::sprintf( response->wr_ptr(), "You Said:  (%s)", message->rd_ptr() );
    response->wr_ptr( strlen(response->wr_ptr())+1 );
    
    message->release();

        // Turn the message around and send it back down the Stream.
    return this->reply( response, timeout );
}

Handler::Handler(void)
{
    ;
}

Handler::~Handler(void)
{
    ;
}

int Handler::open (void *)
{
    ACE_INET_Addr addr;

    if (this->peer ().get_remote_addr (addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't get remote addr\n"), -1);

    if (ACE_Reactor::instance()->register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't register with reactor\n"), -1);

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected with %s\n", addr.get_host_name() ));

    return stream().open( this->peer(), new Handler_Task() );
}

void Handler::destroy (void)
{
    ACE_Reactor::instance()->remove_handler(this,ACE_Event_Handler::READ_MASK|ACE_Event_Handler::DONT_CALL);

    this->peer ().close ();

    delete this;
}

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

int Handler::handle_input (ACE_HANDLE)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Activity from client\n" ));

        // This will cause a blocking read from the peer().  The data
        // will then be pushed through the protocol stream.
    if( stream().get( ) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't get data from protocol stream\n"), -1);
    }
    
    return 0;
}

