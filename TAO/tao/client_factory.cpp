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

// Template specializations which allow the cached connection manager
// to work better.

size_t
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}

int
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::compare_i (const ACE_INET_Addr &a1, 
                                                                        const ACE_INET_Addr &a2) const
{
  return a1 != a2;
}

int
TAO_Client_Connection_Handler::open (void *)
{
  // Here is where we could enable all sorts of things such as
  // nonblock I/O, sock buf sizes, TCP no-delay, etc.
  
  // @@ Chris, this should be changed to a macro that we put in the
  // ACE OS.h file.
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
    // For now, we just return success
    return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
