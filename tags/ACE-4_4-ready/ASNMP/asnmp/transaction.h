/* -*-C++-*- */
#ifndef TRANSACTION_
#define TRANSACTION_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    snmp.h
//
// = DESCRIPTION
//   SNMP class defintion. The Snmp class provides an object oriented
//   approach to SNMP. The SNMP class is an encapsulation of SNMP
//   sessions, gets, sets, etc. The class manages all SNMP
//   resources and provides complete retry and timeout capability.
//
// = AUTHOR
//    Michael R. MacFaden port to ACE / use Reactor pattern 
//
// ============================================================================

#include "ace/Event_Handler.h"
#include "asnmp/target.h"
#include "asnmp/pdu.h"
#include "asnmp/wpdu.h"		// cmu adapter class
#include "ace/SOCK_Dgram.h"

class ACE_Export transaction : public ACE_Event_Handler  
  // = TITLE
  //      Used to manage the details of a particular transaction betwen
  //       two SNMP agents. Uses SnmpTarget class to implement retry/timeout  

{
 public:
  transaction(const Pdu& pdu, const UdpTarget& target, ACE_SOCK_Dgram& io);
  transaction(ACE_SOCK_Dgram& io):session_(io) { }
  // constructor

  ~transaction();
  // destructor

   int run();    
   // begin polling for values

   int result(Pdu& pdu, char *comm_str = 0, ACE_INET_Addr *from_addr = 0) const;
   // return pdu with result from agent after run() is completed rc = 0
   // optionally get community str

   virtual int handle_input (ACE_HANDLE fd);
   // called by reactor when data is ready to be read in from OS memory

   int send();
   // transmit buffer command to network...

   const ACE_INET_Addr& get_from_addr() const;
   // pre: handle_input called
   // retrieve the sender's from address from the last pkt 

 private:
  transaction(const transaction&);
  // disallow copy construction

  wpdu wp_;			// wire pdu
  UdpTarget params_;		// params
  ACE_INET_Addr addr_;		// to address
  ACE_SOCK_Dgram session_;      // io object 
  iovec receive_iovec_;		// receive buffer
  ACE_INET_Addr receive_addr_;	// from address
};

#endif // TRANSACTION_
