/* -*-C++-*- */
#ifndef SNMP_CLS_
#define SNMP_CLS_
//=============================================================================
/**
 *  @file    snmp.h
 *
 *  $Id$
 *
 * SNMP class defintion. The Snmp class provides an object oriented
 * approach to SNMP. The SNMP class is an encapsulation of SNMP
 * sessions, gets, sets, etc. The class manages all SNMP
 * resources and provides complete retry and timeout capability.
 *
 *
 *  @author Peter E Mellquist  design
 *  @author first implementation Michael R. MacFaden port to ACE / use Reactor pattern
 */
//=============================================================================

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
#include "asnmp/transaction_result.h"
#include "asnmp/ASNMP_Export.h"

class Snmp;
class ASNMP_Export Snmp_Result
{
  public:
    Snmp_Result ();
    virtual ~Snmp_Result();
    virtual void result(Snmp *snmp, int result) = 0;
};

// Snmp session class - supports Version 1 operations in blocking mode
/**
 * @class Snmp
 *
 * @brief Concrete class Snmp defined the session and interface to
 * communicate with another SNMP Version 1 agent
 */
class ASNMP_Export Snmp : public transaction_result
{
  Snmp_Result * result_;
  Pdu * pdu_;
  unsigned hold_req_id_;
public:
  Snmp(unsigned short port = INADDR_ANY);
  virtual ~Snmp();

  /// retrieve data from a peer agent for a given list of oid values
  /// default port 161
  int get( Pdu &pdu, UdpTarget &target, Snmp_Result * cb = 0);

  /**
   * retrieve data lexically adjacent to the oids specified in the pdu
   * from the peer agent
   * default port 161
   */
  int get_next( Pdu &pdu, UdpTarget &target, Snmp_Result * cb = 0);

  /// set data in the agent from the list of oids in the pdu
  /// default port 161
  int set( Pdu &pdu, UdpTarget &target, Snmp_Result * cb = 0);

  /// send an SNMPv1 trap (unreliable) to a remote system (def port 162)
  int trap( Pdu &pdu, UdpTarget &target);

  /// status of object after construction
  int valid() const;

  /// given error code, return string reason
  static const char * error_string(int code);

  /// retrieve a reason string if any of the above commands fail
  const char * error_string();

  /// for async transaction results
  void result(transaction * t, int rc);

  /// allow the host name to be overriden
  static void override_host_name(const char* name);

  /// returns the overriden host name
  static void get_host_name(char* name, int len);

protected:
  void check_default_port(UdpTarget& target,unsigned short port=DEF_AGENT_PORT);
  int run_transaction(Pdu& pdu, UdpTarget& target);
  int run_transaction(Pdu& pdu, UdpTarget& target, Snmp_Result * cb);
  int validate_args(const Pdu& pdu, const UdpTarget& target) const;

  Snmp(const Snmp&);

  /// io object
  ACE_SOCK_Dgram iv_snmp_session_;

  /// status of construction
  int construct_status_;

  /// result code from last transaction
  int last_transaction_status_;

  /// transaction request id
  unsigned req_id_;

  static char host_name_[MAXHOSTNAMELEN];
};

#endif //SNMP_CLS_
