// $Id$

// ACE_LSOCK Server.

#include "ace/LSOCK_Acceptor.h"
#include "ace/UNIX_Addr.h"
#include "CPP-acceptor.h"

typedef Svc_Handler<ACE_LSOCK_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_LSOCK_ACCEPTOR> IPC_SERVER;

int 
main (int argc, char *argv[])
{                                                                
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_SERVER peer_acceptor;
  
  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

  return peer_acceptor.svc ();
}


#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Concurrency_Strategy<SVC_HANDLER>;
template class ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_LSOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
template class IPC_Server<SVC_HANDLER, ACE_SOCK_LACCEPTOR>;
template class Svc_Handler<ACE_LSOCK_STREAM>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
