/* -*-C++-*- */
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    transaction.cpp
//
// = DESCRIPTION
//  implements blocking SNMPv1 API using a simple state machine
//   transactions over UDP/IP networks
//
// = AUTHOR
//   Michael R MacFaden  mrm@cisco.com - remove v2c, async, rework for ACE 
// ============================================================================

#include "ace/Reactor.h"
#include "asnmp/transaction.h"

// pre: pdu, target report  valid() == 1
// post: pdu sent out over the wire
inline void reset_receive_buffer(iovec& io)
{
  io.iov_base = 0;
  io.iov_len = 0;
}

transaction::transaction(const Pdu& pdu, const UdpTarget& target, 
			 ACE_SOCK_Dgram& io):
			 wp_(pdu,target), params_(target), session_(io)
{
  // last step, convert address (get ride of this once we have merged address
  UdpAddress udp;
  target.get_address(udp);
  // via string conversion "dotted-quad:port"
  ACE_INET_Addr tmp(udp);
  addr_ = tmp;
  reset_receive_buffer(receive_iovec_);
}

transaction::~transaction()
{
  delete [] receive_iovec_.iov_base;
}

// implement state machine, send, wait (timeout/results) return
int transaction::run()
{
  int rc, done = FALSE;
  unsigned retry_counter = 0;
  ACE_Time_Value to(params_.get_timeout(), 0); // seconds
  ACE_Reactor reactor;

  // 1. register io port for read access
  if (reactor.register_handler(session_.get_handle(), this, 
			       ACE_Event_Handler::READ_MASK) == -1)
    return SNMP_CLASS_INTERNAL_ERROR;

  // register a time handler and a socket with this 
  
  while (!done) {
   
    if ((rc = send()) < 0)	// send pkt to agent
	return rc;
    else {
      if (retry_counter++ > params_.get_retry())
	return SNMP_CLASS_TIMEOUT;
    }

    // 2. wait for events (timeout, returned msg)
    if (( rc = reactor.handle_events (to)) == 1) // one handler registered
      return 0;
    else {
      if (rc == 0) {
         to.set(params_.get_timeout(), 0);
      }
      else
	return SNMP_CLASS_INTERNAL_ERROR;
    }
   }

}

// got back response from SNMPv1 agent - process it
int transaction::handle_input (ACE_HANDLE fd)
{
  // OS allocates iovec_.iov_base ptr and len
  int rc = session_.recv(&receive_iovec_, receive_addr_, 0); 
  if (rc == -1) {
    return SNMP_CLASS_RESOURCE_UNAVAIL;
  }
 
  return rc;
}

// return pdu to caller
int transaction::result(Pdu& pdu)
{
  // TODO: check to see the sender matches the receiver address..

  // remove any vbs existing in this pdu
  pdu.delete_all_vbs();

 // any data to return?
 if (receive_iovec_.iov_len == 0)
   return -1;

 wpdu tmp(receive_iovec_);
 snmp_version ver;
 if (tmp.get_pdu(pdu, ver) == 0)
   return 0;
 else
  return -1;
}

int transaction::send()
{
  iovec io = wp_.get_buffer();
  if (io.iov_len == 0) {
    // NO DATA ?
    return -1;
  }
  ssize_t rc = session_.send (io.iov_base, io.iov_len, addr_ , 0);
  return rc;                
}
