// $Id$

// ACE_SPIPE Client.

#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Addr.h"
#include "CPP-connector.h"

ACE_RCSID(non_blocking, test_spipe_connector, "$Id$")

#if !defined (ACE_WIN32)
typedef Peer_Handler<ACE_SPIPE_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_SPIPE_CONNECTOR> IPC_CLIENT;

int
main (int argc, char *argv[])
{
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
template class ACE_Connector<PEER_HANDLER, ACE_SPIPE_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_SYNCH>;
template class ACE_NonBlocking_Connect_Handler<PEER_HANDLER>;
template class IPC_Client<PEER_HANDLER, ACE_SPIPE_CONNECTOR>;
template class Peer_Handler<ACE_SPIPE_STREAM>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector_Base<PEER_HANDLER>
#pragma instantiate ACE_Connector<PEER_HANDLER, ACE_SPIPE_CONNECTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_SYNCH>
#pragma instantiate ACE_NonBlocking_Connect_Handler<PEER_HANDLER>
#pragma instantiate IPC_Client<PEER_HANDLER, ACE_SPIPE_CONNECTOR>
#pragma instantiate Peer_Handler<ACE_SPIPE_STREAM>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "This test is not ported to Win32 (yet)\n"),
                    -1);
}
#endif /* !ACE_WIN32 */
