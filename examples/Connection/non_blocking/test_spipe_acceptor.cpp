// ACE_SPIPE Server.
// $Id$


#include "ace/SPIPE_Acceptor.h"
#include "ace/SPIPE_Addr.h"
#include "CPP-acceptor.h"

typedef Svc_Handler<ACE_SPIPE_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_SPIPE_ACCEPTOR> IPC_SERVER;

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

