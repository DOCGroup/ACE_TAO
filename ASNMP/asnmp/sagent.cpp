
//=============================================================================
/**
 *  @file    sagent.cpp
 *
 *  $Id$
 *
 * SNMP agent class definition. The sagent class provides an object oriented
 * approach for creating SNMP Agents. The sagent class is an encapsulation
 * of SNMP sessions, gets, sets, etc.
 *
 *
 *  @author Michael R. MacFaden
 */
//=============================================================================

#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"

#include "asnmp/oid.h"                // snmp++ oid class
#include "asnmp/vb.h"                 // snbmp++ vb class
#include "asnmp/target.h"             // snmp++ target class
#include "asnmp/pdu.h"                // snmp++ pdu class
#include "asnmp/snmperrs.h"           // error macros and strings
#include "asnmp/address.h"            // snmp++ address class defs
#include "asnmp/snmp.h"               // manager snmp interface
#include "asnmp/sagent.h"             // agent interface
#include "asnmp/transaction.h"        // convert from wire to API

sagent::sagent(unsigned short port): Snmp(port)
{
  ACE_TRACE("sagent::sagent(short)");
}

sagent::~sagent()
{
  ACE_TRACE("sagent::~sagent");
}

int sagent::handle_input(ACE_HANDLE fd)
{
  ACE_TRACE("sagent::handle_input");

  transaction tr(iv_snmp_session_); // this section needs a better design
  tr.handle_input(fd);
  char rcv_com_str[MAX_COMM_STR_LEN];
  if (tr.result(pdu_, rcv_com_str) < 0)
     return 0;
  OctetStr community(rcv_com_str);
  const ACE_INET_Addr &ta = tr.get_from_addr();
  ACE_TCHAR buf_tmp[MAXHOSTNAMELEN + 1];
  ta.addr_to_string (buf_tmp, MAXHOSTNAMELEN);
  UdpAddress ra (ACE_TEXT_ALWAYS_CHAR (buf_tmp));
  tgt_.set_address(ra);


  // process msg here by calling subclass's implementation
  switch (pdu_.get_type()){
  case sNMP_PDU_GET:
    tgt_.set_read_community(community);
    this->handle_get(pdu_, tgt_);
    break;

  case sNMP_PDU_GETNEXT:
    tgt_.set_read_community(community);
    this->handle_get_next(pdu_, tgt_);
    break;

  case sNMP_PDU_SET:
    tgt_.set_write_community(community);
    this->handle_set(pdu_, tgt_);
    break;

  default:
    ACE_ASSERT(0);
  }
  return 0;
}

ACE_HANDLE sagent::get_handle() const
{
  ACE_TRACE("sagent::get_handle");
  return iv_snmp_session_.get_handle();
}

int sagent::respond(Pdu& pdu,UdpTarget& tgt)
{
  pdu.set_type(sNMP_PDU_RESPONSE);
  transaction tr(pdu, tgt, iv_snmp_session_);
  tr.send();
  return 0;
}
