// ACE_TLI Server.
// $Id$


#include "ace/TLI_Acceptor.h"
#include "ace/INET_Addr.h"
#include "CPP-acceptor.h"

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

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Concurrency_Strategy<SVC_HANDLER>;
template class ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_TLI_ACCEPTOR>;
template class ACE_Message_Queue<ACE_NULL_SYNCH>;
template class ACE_Module<ACE_NULL_SYNCH>;
template class ACE_Svc_Handler<ACE_TLI_STREAM, ACE_NULL_SYNCH>;
template class ACE_TSS<ACE_Dynamic>;
template class ACE_Task<ACE_NULL_SYNCH>;
template class ACE_Thru_Task<ACE_NULL_SYNCH>;
template class IPC_Server<SVC_HANDLER, ACE_TLI_ACCEPTOR>;
template class Svc_Handler<ACE_TLI_STREAM>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support TLI\n"), 1);
}
#endif /* ACE_HAS_TLI */
