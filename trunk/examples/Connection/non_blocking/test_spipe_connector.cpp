// ACE_SPIPE Client.
// $Id$

#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Addr.h"
#include "CPP-connector.h"

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
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);
  
  return peer_connector.svc ();
}                                                       

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Connector<PEER_HANDLER, ACE_SPIPE_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_SYNCH>;
template class ACE_Svc_Tuple<PEER_HANDLER>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<PEER_HANDLER> *, ACE_SYNCH_RW_MUTEX>;
template class IPC_Client<PEER_HANDLER, ACE_SPIPE_CONNECTOR>;
template class Peer_Handler<ACE_SPIPE_STREAM>;

#if defined (ACE_HAS_THREADS)
  template class ACE_Guard<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#else
  // These are specialized in libACE if ACE doesn't have threads.
#endif /* ACE_HAS_THREADS */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "This test is not ported to Win32 (yet)\n"), -1);
}
#endif /* !ACE_WIN32 */
