// $Id$

#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Service_Config.h"

#include "CPP-acceptor.h"

typedef Svc_Handler<ACE_SOCK_STREAM> SVC_HANDLER;
typedef IPC_Server<SVC_HANDLER, ACE_SOCK_ACCEPTOR> IPC_SERVER;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_SERVER peer_acceptor;

  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("init")),
                      -1);

  return peer_acceptor.svc ();
}

