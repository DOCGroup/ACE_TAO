/* -*-C++-*- */
#ifndef SAGENT_CLS_
#define SAGENT_CLS_
//=============================================================================
/**
 *  @file    sagent.h
 *
 *  $Id$
 *
 * SNMP agent class defintion. The sagent class provides an object oriented
 * approach for creating SNMP Agents. The sagent class is an encapsulation of SNMP
 * sessions, gets, sets, etc.
 *
 *  @author Michael R. MacFaden
 */
//=============================================================================

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Dgram.h"

#include "asnmp/oid.h"                // snmp++ oid class
#include "asnmp/vb.h"                 // snbmp++ vb class
#include "asnmp/target.h"             // snmp++ target class
#include "asnmp/pdu.h"                // snmp++ pdu class
#include "asnmp/snmperrs.h"           // error macros and strings
#include "asnmp/address.h"            // snmp++ address class defs
#include "asnmp/snmp.h"               // snmp interface

// sagent - supports Version 1 operations in blocking mode

#define DEF_AGENT_PORT (161)

/**
 * @class sagent
 *
 * @brief Concrete class sagent defines the session and interface to
 * communicate with another SNMP Version 1 manager
 */
class ASNMP_Export sagent : public ACE_Event_Handler, private Snmp
{
public:
    // override the next three methods (callbacks) to implement your agent
    //

  /// retrieve data from a peer agent for a given list of oid values
  virtual int handle_get( Pdu &pdu, UdpTarget &target) = 0;

  /// retrieve data lexically adjacent to the oids specified in the pdu
  /// from the peer agent
  virtual int handle_get_next( Pdu &pdu, UdpTarget &target) = 0;

  /// set data in the agent from the list of oids in the pdu
  virtual int handle_set( Pdu &pdu, UdpTarget &target) = 0;

  /// new pdu received from mgr (reactor calls this)
  virtual int handle_input(ACE_HANDLE);

  /// retrieve io descriptor (reactor uses this)
  virtual ACE_HANDLE get_handle() const;

  /// send a response pdu to the mgr
  int respond(Pdu& pdu, UdpTarget& tgt);

protected:
  sagent(unsigned short port = DEF_AGENT_PORT);
  virtual ~sagent();

private:
  sagent(const sagent&);
  Pdu pdu_;                     // current input pdu
  UdpTarget tgt_;               //  addr & com strs
};

#endif //SAGENT_CLS_

