
// $Id$

#include "Server_i.h"
#include "ace/Select_Reactor.h"

// A signal handler that will close the server object
extern "C" void handler (int)
{
    Server::close();
}

int main (int, char **)
{
        /* On Win32, the WFMO reactor is used by default.
           Unfortunately, that causes the sockets to be put into
           non-blocking mode which will break Recv::recv().  To
           prevent that issue, I explicitly use the Select Reactor
           instead.  I'll talk more about the "problem" in the Recv
           comments.
         */

        // First, we create a Select_Reactor that will do the work.
        // To keep things simple, I'll create it on the stack.
    ACE_Select_Reactor mySelectReactor;

        // Next, we need an ACE_Reactor that is the bridge between the
        // code and the real reactor.  It is given a pointer to the
        // real reactor.
    ACE_Reactor myReactor (&mySelectReactor);

        // Finally, we set the singleton instance to use the reactor
        // we've created.
    ACE_Reactor::instance (&myReactor);

        // The server object that abstracts away all of the difficult parts.
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

