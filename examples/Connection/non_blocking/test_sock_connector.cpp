// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "CPP-connector.h"

typedef Peer_Handler<ACE_SOCK_STREAM> PEER_HANDLER;
typedef IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR> IPC_CLIENT;

// ACE_SOCK Client.
                                                        
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
template class ACE_Connector<PEER_HANDLER, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
template class ACE_Svc_Tuple<PEER_HANDLER>;
template class ACE_TSS<ACE_Dynamic>;
template class IPC_Client<PEER_HANDLER, ACE_SOCK_CONNECTOR>;
template class Peer_Handler<ACE_SOCK_STREAM>;
template class ACE_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<int, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
