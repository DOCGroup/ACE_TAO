// $Id$

// ACE_SPIPE Client.

#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Addr.h"
#include "CPP-connector.h"



#if !defined (ACE_WIN32)
typedef Peer_Handler<ACE_SPIPE_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_SPIPE_CONNECTOR> IPC_CLIENT;

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

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "This test is not ported to Win32 (yet)\n"),
                    -1);
}
#endif /* !ACE_WIN32 */
