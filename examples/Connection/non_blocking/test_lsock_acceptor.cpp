// $Id$

// ACE_LSOCK Server.

#include "ace/LSOCK_Acceptor.h"

#if defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
int
main (int, char *argv[])
{
  ACE_ERROR_RETURN ((LM_INFO,
                     ACE_TEXT ("%s: not supported with ")
                     ACE_TEXT ("ACE_LACKS_UNIX_DOMAIN_SOCKETS\n"),
                     argv[0]),
                     -1);
}

#else  /* ! ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#include "ace/UNIX_Addr.h"
#include "CPP-acceptor.h"

ACE_RCSID(non_blocking, test_lsock_acceptor, "$Id$")

typedef Svc_Handler<ACE_LSOCK_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_LSOCK_ACCEPTOR> IPC_SERVER;

int
main (int argc, char *argv[])
{
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_SERVER peer_acceptor;

  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init"),
                      -1);
  return peer_acceptor.svc ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Concurrency_Strategy<SVC_HANDLER>;
template class ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_LSOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
template class IPC_Server<SVC_HANDLER, ACE_LSOCK_ACCEPTOR>;
template class Svc_Handler<ACE_LSOCK_STREAM>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Concurrency_Strategy<SVC_HANDLER>
#pragma instantiate ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate IPC_Server<SVC_HANDLER, ACE_LSOCK_ACCEPTOR>
#pragma instantiate Svc_Handler<ACE_LSOCK_STREAM>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ! ACE_LACKS_UNIX_DOMAIN_SOCKETS */
