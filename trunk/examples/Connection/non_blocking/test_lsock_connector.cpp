// $Id$

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "CPP-connector.h"

typedef Peer_Handler<ACE_LSOCK_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_LSOCK_CONNECTOR> IPC_CLIENT;

// ACE_LSOCK Client.
                                                        
int 
main (int argc, char *argv[])                       
{                                                       
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_CLIENT peer_connector;

  if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);
  
  return peer_connector.svc ();
}                                                       

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Connector<PEER_HANDLER, ACE_LSOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_SYNCH>;
template class ACE_Svc_Tuple<PEER_HANDLER>;
template class IPC_Client<PEER_HANDLER, ACE_LSOCK_CONNECTOR>;
template class Peer_Handler<ACE_LSOCK_STREAM>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
