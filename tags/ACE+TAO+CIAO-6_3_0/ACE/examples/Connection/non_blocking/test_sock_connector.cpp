// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "CPP-connector.h"



typedef Peer_Handler<ACE_SOCK_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR> IPC_CLIENT;

// ACE_SOCK Client.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Since this test waits on the STDIN handle to become ready, we
  // have to make sure that the WFMO_Reactor is used on Win32. This is
  // necessary since <select> on NT does not support waiting on STDIN.

#if defined (ACE_WIN32)
# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  ACE_WFMO_Reactor wfmo_reactor;
  ACE_Reactor reactor (&wfmo_reactor);
  ACE_Reactor::instance (&reactor);
# else
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Non-NT platform (Win95/98?) without Winsock2 ")
                     ACE_TEXT ("installed.\n")
                     ACE_TEXT ("This example requires WFMO_Reactor which ")
                     ACE_TEXT ("requires.\n")
                     ACE_TEXT ("Winsock2 be installed.\n")), -1);
# endif /*  !ACE_HAS_WINSOCK2 && ACE_HAS_WINSOCK2 != 0 */
#endif /* ACE_WIN32 */

  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_CLIENT peer_connector;

  if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("init")),
                      -1);

  return peer_connector.svc ();
}

