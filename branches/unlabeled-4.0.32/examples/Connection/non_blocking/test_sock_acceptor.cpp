// ACE_SOCK Server.
// @(#)test_sock_acceptor.cpp	1.1	10/18/96


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

  IPC_SERVER acceptor;
  
  if (acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

  return acceptor.svc ();
}

