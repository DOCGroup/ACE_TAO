// ACE_SOCK Server.
// $Id$


#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "CPP-acceptor.h"

typedef Svc_Handler<ACE_SOCK_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_SOCK_ACCEPTOR> IPC_SERVER;

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
template class IPC_Server<SVC_HANDLER, ACE_SOCK_ACCEPTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
