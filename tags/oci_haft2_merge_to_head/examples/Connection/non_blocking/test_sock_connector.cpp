// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "CPP-connector.h"

ACE_RCSID(non_blocking, test_sock_connector, "$Id$")

typedef Peer_Handler<ACE_SOCK_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR> IPC_CLIENT;

// ACE_SOCK Client.

int
main (int argc, char *argv[])
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
                     "Non-NT platform (Win95/98?) without Winsock2 installed.\n"
                     "This example requires WFMO_Reactor which requires.\n"
                     "Winsock2 be installed.\n"), -1);
# endif /*  !ACE_HAS_WINSOCK2 && ACE_HAS_WINSOCK2 != 0 */
#endif /* ACE_WIN32 */

  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_CLIENT peer_connector;

  if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init"),
                      -1);

  return peer_connector.svc ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector_Base<PEER_HANDLER>;
template class ACE_Connector<PEER_HANDLER, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
template class ACE_NonBlocking_Connect_Handler<PEER_HANDLER>;
template class IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR>;
template class Peer_Handler<ACE_SOCK_STREAM>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector_Base<PEER_HANDLER>
#pragma instantiate ACE_Connector<PEER_HANDLER, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
#pragma instantiate ACE_NonBlocking_Connect_Handler<PEER_HANDLER>
#pragma instantiate IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR>
#pragma instantiate Peer_Handler<ACE_SOCK_STREAM>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
