// $Id$

// ACE_TLI Server.

#include "ace/TLI_Acceptor.h"
#include "ace/INET_Addr.h"
#include "CPP-acceptor.h"

ACE_RCSID(non_blocking, test_tli_acceptor, "$Id$")

#if defined (ACE_HAS_TLI)
typedef Svc_Handler<ACE_TLI_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_TLI_ACCEPTOR> IPC_SERVER;

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Concurrency_Strategy<SVC_HANDLER>;
template class ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_TLI_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_TLI_STREAM, ACE_NULL_SYNCH>;
template class IPC_Server<SVC_HANDLER, ACE_TLI_ACCEPTOR>;
template class Svc_Handler<ACE_TLI_STREAM>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Concurrency_Strategy<SVC_HANDLER>
#pragma instantiate ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_TLI_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_TLI_STREAM, ACE_NULL_SYNCH>
#pragma instantiate IPC_Server<SVC_HANDLER, ACE_TLI_ACCEPTOR>
#pragma instantiate Svc_Handler<ACE_TLI_STREAM>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support TLI\n"), 1);
}
#endif /* ACE_HAS_TLI */
