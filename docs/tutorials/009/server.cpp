
// $Id$

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int main(int argc,char *argv[] )
{
	ACE_INET_Addr  local(PORT);
	ACE_SOCK_Dgram dgram;

	if( dgram.open(local) == -1 )
	{
		cerr << "Listener failed to open" << endl;
		return(0);
	}

	char buf[512];
	ACE_INET_Addr remote;

	while( dgram.recv(buf,sizeof(buf),remote) != -1 )
	{
		cerr << "Received:  " << buf << " from " << remote.get_host_name() << endl;

		if( argc > 1 )
		{
			/* Respond to the client's query
			 */
			if( ACE_OS::strcmp(buf,argv[1]) )
			{
				cerr << "Client query does not match our signature" << endl;
			}
			else
			{
				ACE_INET_Addr  local((u_short)0);
				ACE_SOCK_Dgram client;
				if( client.open(local) == -1 )
				{
					cerr << "Cannot open response dgram" << endl;
					return(0);
				}

				sprintf(buf,"I am here");
				if( client.send(buf,strlen(buf)+1,remote) == -1 )
				{
					cerr << "Failed to send response to client" << endl;
					return(0);
				}
			}
		}
	}

	return(0);
}
