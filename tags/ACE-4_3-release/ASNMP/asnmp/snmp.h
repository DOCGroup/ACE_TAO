/* -*-C++-*- */
#ifndef SNMP_CLS_
#define SNMP_CLS_
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
//    Peter E Mellquist  design, first implementation
//    Michael R. MacFaden port to ACE / use Reactor pattern 
//
// ============================================================================
/*===================================================================
   Copyright (c) 1996
   Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software 
  and/or its documentation is hereby granted without fee. User agrees 
  to display the above copyright notice and this license notice in all 
  copies of the software and any documentation of the software. User 
  agrees to assume all liability for the use of the software; Hewlett-Packard 
  makes no representations about the suitability of this software for any 
  purpose. It is provided "AS-IS without warranty of any kind,either express 
  or implied. User hereby grants a royalty-free license to any and all 
  derivatives based upon this software code base. 
=====================================================================*/

#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"

#include "asnmp/oid.h"                // snmp++ oid class
#include "asnmp/vb.h"                 // snbmp++ vb class
#include "asnmp/target.h"             // snmp++ target class
#include "asnmp/pdu.h"                // snmp++ pdu class
#include "asnmp/snmperrs.h"           // error macros and strings 
#include "asnmp/address.h"            // snmp++ address class defs

// Snmp session class - supports Version 1 operations in blocking mode 
class ACE_Export Snmp 
  // = TITLE
  //      Concrete class Snmp defined the session and interface to
  //      communicate with another SNMP Version 1 agent 
{
 public:
    Snmp();
   ~Snmp();

    int get( Pdu &pdu, UdpTarget &target);
    // retrieve data from a peer agent for a given list of oid values
    // default port 161

    int get_next( Pdu &pdu, UdpTarget &target); 
    // retrieve data lexically adjacent to the oids specified in the pdu
    // from the peer agent 
    // default port 161

    int set( Pdu &pdu, UdpTarget &target); 
    // set data in the agent from the list of oids in the pdu
    // default port 161

    int trap( Pdu &pdu, UdpTarget &target);
    // send an SNMPv1 trap (unreliable) to a remote system (def port 162)

    int valid() const; 
    // status of object after construction 

    static char * error_string(int code);  
    // given error code, return string reason

    char * error_string();  
    // retrieve a reason string if any of the above commands fail

private:
  void check_default_port(UdpTarget& target,unsigned short port=DEF_AGENT_PORT);
  int run_transaction(Pdu& pdu, UdpTarget& target);
  int validate_args(const Pdu& pdu, const UdpTarget& target) const;

  Snmp(const Snmp&);

  ACE_SOCK_Dgram iv_snmp_session_;    
  // io object 

  int construct_status_;              
  // status of construction

  int last_transaction_status_;       
  // result code from last transaction 

  unsigned req_id_;                   
  // transaction request id 
};

#endif //SNMP_CLS_

