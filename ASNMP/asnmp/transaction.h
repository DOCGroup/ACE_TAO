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
  // constructor

  ~transaction();
  // destructor

   int run();    
   // begin polling for values

   int result(Pdu& pdu); 
   // return pdu with result from agent after run() is completed rc = 0

   virtual int handle_input (ACE_HANDLE fd);
   // called by reactor when data read on session_

   int send();
   // transmit buffer command to network...

 private:
  transaction(const transaction&);
  // disallow copy construction

  wpdu wp_;
  UdpTarget params_;		
  ACE_INET_Addr addr_;
  ACE_SOCK_Dgram session_;    
  // io object 

  iovec receive_iovec_;	// receive buffer
  // incomming msg details
  ACE_Addr receive_addr_;	
  // address msg received from
};

#endif // TRANSACTION_
