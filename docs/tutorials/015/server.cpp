
// $Id$

#include "Server.h"

// A signal handler that will close the server object
extern "C" void handler (int)
{
    Server::close();
}

int main (int, char **)
{
        // The server object that abstracts away all of difficult parts.
    Server server;

        // Attempt to open the server.  Like all good ACE-based
        // objects, we'll get -1 on failure.
    if( server.open() == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.open()"), -1);
    }

        // Install a signal handler for ^C so that we can exit gracefully
    ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

        // Run the server's main loop until we're interrupted
    if( server.run() == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), -1);
    }

    return 0;
}

/* These explicit instantiations were taken from an earlier tutorial.
   Your compiler may require others as well.
*/
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

