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

  IPC_SERVER acceptor;
  
  if (acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

  return acceptor.svc ();
}
#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support TLI\n"), 1);
}
#endif /* ACE_HAS_TLI */

