// ACE_SPIPE Client.
// @(#)test_spipe_connector.cpp	1.1	10/18/96


#include "ace/SPIPE_Connector.h"
#include "ace/SPIPE_Addr.h"
#include "SPIPE-connector.h"

int 
main (int argc, char *argv[])                       
{                                                       
  // Perform Service_Config initializations
  ACE_Service_Config daemon (argv[0]);

  IPC_Client peer_connector;

  if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);
  
  return peer_connector.svc ();
}                                                       

