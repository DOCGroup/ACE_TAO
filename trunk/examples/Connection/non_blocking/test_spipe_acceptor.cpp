// ACE_SPIPE Server.
// $Id$

#include "ace/SPIPE_Acceptor.h"
#include "ace/SPIPE_Addr.h"
#include "CPP-acceptor.h"

#if !defined (ACE_WIN32)
typedef Svc_Handler<ACE_SPIPE_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_SPIPE_ACCEPTOR> IPC_SERVER;

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
template class IPC_Server<SVC_HANDLER, ACE_SPIPE_ACCEPTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "This test is not ported to Win32 (yet)\n"), -1);
}
#endif /* !ACE_WIN32 */

