
//=============================================================================
/**
 *  @file    snmp.cpp
 *
 *  $Id$
 *
 * The basic SNMPv1 API - blocking version using UDP/IPv4 transport
 * and addressing.
 *
 *
 *  @author Peter E Mellquist class design/orig codeMichael R MacFaden  mrm@cisco.com / rework API
 *  @author use ACE facilities
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


#define INCLUDE_SNMP_ERR_STRINGS_

//----[ snmp++ includes ]----------------------------------------------
#include "asnmp/snmp.h"    // class def for this module
#include "asnmp/oid.h"     // class def for oids
#include "asnmp/enttraps.h" // class def for well known trap oids
#include "asnmp/vb.h"        // class def for vbs
#include "asnmp/address.h" // class def for addresses
#include "asnmp/wpdu.h"    // adapter to cmu_library / wire protocol
#include "asnmp/transaction.h" // synchronous transaction processor
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

const coldStartOid coldStart;
const warmStartOid warmStart;
const linkDownOid linkDown;
const linkUpOid linkUp;
const authenticationFailureOid authenticationFailure;
const egpNeighborLossOid egpNeighborLoss;
const snmpTrapEnterpriseOid snmpTrapEnterprise;

char Snmp::host_name_[MAXHOSTNAMELEN] = "";

Snmp::Snmp(unsigned short port): result_(0), construct_status_(SNMP_CLASS_ERROR), last_transaction_status_(0)
{
  ACE_TRACE("Snmp::Snmp");

  ACE_INET_Addr addr(port); // any port,address is ok
  if (iv_snmp_session_.open(addr) < 0) {
     last_transaction_status_ = errno; // open udp/ipv4 socket
     ACE_DEBUG((LM_DEBUG, "Snmp::snmp::open port %d failed", port));
     return;
  }

  // initial request id randomly generated then monotonically incremented
  req_id_ = (unsigned) ACE_OS::time(0);

  construct_status_ = SNMP_CLASS_SUCCESS;
  return;
}

Snmp::~Snmp()
{
  ACE_TRACE("Snmp::~Snmp");
  iv_snmp_session_.close();
}

int Snmp::valid() const
{
  return construct_status_;
}

// given error code, return string definition
// class version
// static
const char * Snmp::error_string(int last_transaction_status_)
{
  ACE_TRACE("Snmp::error_string");

  return ((last_transaction_status_ < 0) ?
           ((last_transaction_status_ < MAX_NEG_ERROR)?
        nErrs[ abs(MAX_NEG_ERROR) + 1] : nErrs[abs(last_transaction_status_)]):
            ((last_transaction_status_ > MAX_POS_ERROR)?
        pErrs[MAX_POS_ERROR+1] : pErrs[last_transaction_status_]));
}

// instance version
const char * Snmp::error_string()
{
   return Snmp::error_string(last_transaction_status_);
}

void Snmp::check_default_port(UdpTarget& target, unsigned short port)
{
  UdpAddress tmp;
  target.get_address(tmp);
  if (tmp.get_port() == 0) {
    tmp.set_port(port);
    target.set_address(tmp);
  }
}

int Snmp::run_transaction(Pdu& pdu, UdpTarget& target)
{
  int rc, done = 0;

  // 1. set unique id to match this packet on return
  size_t hold_req_id = req_id_++;
  set_request_id(&pdu, hold_req_id);

  // 2. write request to agent
  transaction trans(pdu, target, iv_snmp_session_);

  // this call blocks while it attempts to retrieve agent response
  while (!done) {
    if ((rc = trans.run()) < 0) {
       last_transaction_status_ = rc;
       return rc;
    }
    else {
      trans.result(pdu);
      // verify this is the pdu we are after
      if (pdu.get_request_id() == hold_req_id)
        done = 1 ;
    }
  }
  return 0;
}

int Snmp::run_transaction(Pdu& pdu, UdpTarget& target, Snmp_Result * cb)
{
  if (!cb)
     return run_transaction(pdu, target);

  // 1. set unique id to match this packet on return
  hold_req_id_ = req_id_++;
  set_request_id(&pdu, hold_req_id_);
  pdu_ = &pdu;
  result_ = cb;

  // 2. write request to agent
  transaction * trans = new transaction(pdu, target, iv_snmp_session_);
  return trans->run(this);
}

void Snmp::result(transaction *t, int rc)
{
    t->result(*pdu_);
    // verify this is the pdu we are after
    if (pdu_->get_request_id() == hold_req_id_)
    {
        last_transaction_status_ = rc;
        delete t;
        result_->result(this, rc);
    }
    else
    {
        rc = t->run(this);
        if (rc < 0)
        {
            delete t;
            result_->result(this, rc);
        }
    }
}

int Snmp::validate_args(const Pdu& pdu, const UdpTarget& target) const
{
  // 0. check object status
  if (construct_status_ != SNMP_CLASS_SUCCESS)
     return construct_status_;

  // 1. check args passed
  if ( !pdu.valid() || !target.valid() )
     return SNMP_INVALID_ARGS;
  return 0;
}

// SYNC API: write request to wire then wait for reply or timeout
int Snmp::get( Pdu &pdu, UdpTarget &target, Snmp_Result * cb)
{
   ACE_TRACE("Snmp::get");
  int rc;
  if ((rc = validate_args(pdu, target)) != 0)
     return rc;

  pdu.set_type( sNMP_PDU_GET);
  check_default_port(target);
  return run_transaction(pdu, target, cb);
}

int Snmp::get_next( Pdu &pdu, UdpTarget &target, Snmp_Result * cb)
{
  ACE_TRACE("Snmp::get_next");
  int rc;
  if ((rc = validate_args(pdu, target)) != 0)
     return rc;

  pdu.set_type( sNMP_PDU_GETNEXT);
  check_default_port(target);
  return run_transaction(pdu, target, cb);
}

int Snmp::set( Pdu &pdu, UdpTarget &target, Snmp_Result * cb)
{
  ACE_TRACE("Snmp::set");
  int rc;
  if ((rc = validate_args(pdu, target)) != 0)
     return rc;

   pdu.set_type( sNMP_PDU_SET);
   check_default_port(target);
   return run_transaction(pdu, target, cb);
}

// one way, best of luck, non-confirmed alert
int Snmp::trap( Pdu &pdu, UdpTarget &target)
{
  ACE_TRACE("Snmp::trap");
  int rc;
  if ((rc = validate_args(pdu, target)) != 0)
     return rc;

  pdu.set_type( sNMP_PDU_V1TRAP);
  check_default_port(target, DEF_TRAP_PORT);

  // 2. write request to agent
  transaction trans(pdu, target, iv_snmp_session_);
  if (trans.send() > 0)  // expect number of bytes sent on
    return 0;

  last_transaction_status_ = SNMP_CLASS_INTERNAL_ERROR;
  return -1;
}

// Allow host name to be overriden. Supplying a null pointer or zero
// length string removes the override.
void Snmp::override_host_name(const char* name)
{
  if (name)
  {
    ACE_OS::strncpy(host_name_, name, MAXHOSTNAMELEN);
    host_name_[MAXHOSTNAMELEN-1] = 0;
  }
  else {
    host_name_[0] = 0;
  }
}

// Returns the current host name in the supplied string.
void Snmp::get_host_name(char* name, int len)
{
  if (name)
  {
    if (ACE_OS::strlen(host_name_) > 0)
    {
      ACE_OS::strncpy(name, host_name_, len);
      name[len-1] = 0;
    }
    else
    {
      if (ACE_OS::hostname(name, len-1) == -1)
        name[0] = 0;
    }
  }
}

Snmp_Result::~Snmp_Result()
{
}

Snmp_Result::Snmp_Result()
{
}
