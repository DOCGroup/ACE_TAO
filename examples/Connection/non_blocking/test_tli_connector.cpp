// $Id$

// ACE_TLI Client.

#include "ace/TLI_Connector.h"
#include "ace/INET_Addr.h"
#include "CPP-connector.h"



#if defined (ACE_HAS_TLI)

typedef Peer_Handler<ACE_TLI_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_TLI_CONNECTOR> IPC_CLIENT;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
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

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("your platform does not support TLI\n")),
                    1);
}
#endif /* ACE_HAS_TLI */
