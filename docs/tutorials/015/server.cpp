
// $Id$

#include "Server.h"

extern "C" void handler (int)
{
    Server::close();
}

int main (int, char **)
{
    Server server;

    if( server.open() == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.open()"), -1);
    }

    ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

    if( server.run() == -1 )
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), -1);
    }

    return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor <Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

