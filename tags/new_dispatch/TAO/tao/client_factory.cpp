// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     client_factory.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if 0
#if !defined (__ACE_INLINE__)
#  include "client_factory.i"
#endif /* __ACE_INLINE__ */
#endif /* 0 */

#include "tao/corba.h"

int
TAO_Client_Connection_Handler::open (void *)
{
  // Here is where we could enable all sorts of things such as
  // nonblock I/O, sock buf sizes, TCP no-delay, etc.
  
  // @@ Chris, this should be changed to a macro that we put in the
  // ACE OS.h file.
#if !defined (VXWORKS)
  const int MAX_SOCK_BUF_SIZE = 65536;

  if (this->peer ().set_option (SOL_SOCKET,
				SO_SNDBUF, 
				(void *)&MAX_SOCK_BUF_SIZE,
				sizeof (MAX_SOCK_BUF_SIZE)) == -1) 
    return -1;

  else if (this->peer ().set_option (SOL_SOCKET,
				     SO_RCVBUF,
				     (void *) &MAX_SOCK_BUF_SIZE,
				     sizeof (MAX_SOCK_BUF_SIZE)) == -1)
    return -1;
  else
#endif
    // For now, we just return success
    return 0;
}

#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_Client_Connection_Handler>
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class TAO_SVC_TUPLE;
template class ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int, TAO_SVC_TUPLE*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate TAO_SVC_TUPLE
#pragma instantiate ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int, TAO_SVC_TUPLE*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
