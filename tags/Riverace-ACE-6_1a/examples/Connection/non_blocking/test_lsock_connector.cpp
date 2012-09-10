// $Id$

#include "ace/LSOCK_Connector.h"
#include "ace/Log_Msg.h"

#if defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_ERROR_RETURN ((LM_INFO,
                     ACE_TEXT ("%s: not supported with ")
                     ACE_TEXT ("ACE_LACKS_UNIX_DOMAIN_SOCKETS\n"),
                     argv[0]),
                     -1);
}

#else  /* ! ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#include "ace/UNIX_Addr.h"
#include "CPP-connector.h"



typedef Peer_Handler<ACE_LSOCK_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_LSOCK_CONNECTOR> IPC_CLIENT;

// ACE_LSOCK Client.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_CLIENT peer_connector;

  if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("init")),
                      -1);

  return peer_connector.svc ();
}

#endif /* ! ACE_LACKS_UNIX_DOMAIN_SOCKETS */
