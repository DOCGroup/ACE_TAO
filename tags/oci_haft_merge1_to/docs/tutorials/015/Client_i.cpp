
// $Id$

#include "Client_i.h"
#include "ace/Message_Block.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"

// Simple constructor just remembers the endpoint information for use by open.
Client::Client( u_short _port, const char * _server)
        : port_(_port), server_(_server)
{
    ;
}

/* Do nothing.  This should probably call close() if we can make sure
   that it's OK to close() multiple times.
*/
Client::~Client(void)
{
    ;
}

/* Open the connection to the server.  This is traditional ACE.  We
   simply construct an endpoint and use a connector to establish the
   link.
*/
int Client::open( void )
{
    ACE_INET_Addr addr(port_,server_);
    ACE_SOCK_Connector con;

    if( con.connect(peer(),addr) == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_SOCK_Connector::connect()"), -1);
    }

        // Something new here...  We have to use the protocol stream
        // to ensure that our data is in the correct format when
        // received by the server.  Thus, we open the stream and
        // transfer ownership of the peer.
    return stream().open( peer() );
}

// The remainder of the functions just delegate to the stream.

int Client::close( void )
{
    return stream().close();
}

int Client::put( ACE_Message_Block * _message )
{
    return stream().put(_message,0);
}

int Client::get( ACE_Message_Block * & _response )
{
    return stream().get(_response);
}
