
//=============================================================================
/**
 *  @file     target.cpp
 *
 *  $Id$
 *
 * Target class defines target SNMP agents.
 *
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
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

#include "asnmp/target.h"
#include "ace/OS_NS_stdio.h"

//----------------------------------------------------------------------
//--------[ Class default values ]----------------------
//----------------------------------------------------------------------
u_long SnmpTarget::default_timeout_ = DEF_TIMEOUT;
int SnmpTarget::default_retries_ = DEF_RETRIES;
u_long SnmpTarget::default_max_pdu_size_ = DEF_MAX_SNMP_PACKET;
snmp_version SnmpTarget::default_version_ = version1;

OctetStr UdpTarget::default_rd_community_(READ_COMM_STR);
OctetStr UdpTarget::default_wr_community_(WRITE_COMM_STR);

//----------------------------------------------------------------------
//--------[ Abstract SnmpTarget Member Functions ]----------------------
//----------------------------------------------------------------------
SnmpTarget::SnmpTarget():
  validity_(0), timeout_(default_timeout_), retries_(default_retries_),
  max_pdu_size_(default_max_pdu_size_), version_(default_version_)
{
}

// return validity of target
int SnmpTarget::valid() const
{
  return validity_;
}

// allow destruction of derived classes
SnmpTarget::~SnmpTarget()
{
}

// set the timeout
void SnmpTarget::set_timeout( const u_long t)
{
  timeout_ = t;
}

// change the default timeout
void SnmpTarget::set_default_timeout( const u_long t)
{
  default_timeout_ = t;
}

// set the retry value
void SnmpTarget::set_retry( const int r)
{
  retries_ = r;
}

// change the default retries
void SnmpTarget::set_default_retry( const int r)
{
  default_retries_ = r;
}

void SnmpTarget:: set_max_pdu_size(const u_long max_pdu_sz)
{
  max_pdu_size_ = max_pdu_sz;
}


void SnmpTarget::set_default_max_pdu_size(const u_long max_pdu_sz)
{
  default_max_pdu_size_ = max_pdu_sz;
}

void SnmpTarget::set_version( const snmp_version v)
{
  version_ = v;
}

void SnmpTarget::set_default_version( const snmp_version v)
{
  default_version_ = v;
}

snmp_version SnmpTarget::get_version() const
{
  return version_;
}

snmp_version SnmpTarget::get_default_version() const
{
  return default_version_;
}

u_long SnmpTarget::get_max_pdu_size() const
{
  return max_pdu_size_;
}

// get the retry value
int SnmpTarget::get_retry() const
{
  return retries_;
}

// get the retry value
int SnmpTarget::get_default_retry() const
{
  return default_retries_;
}

// get the timeout
u_long SnmpTarget::get_timeout() const
{
  return timeout_;
}

SnmpTarget& SnmpTarget::operator=(const SnmpTarget& lhs)
{
  if (this == &lhs)
    return *this;

  validity_ = lhs.validity_;
  timeout_ = lhs.timeout_;
  retries_ = lhs.retries_;
  max_pdu_size_ =lhs.max_pdu_size_;
  version_ = lhs.version_;
  return *this;
}

bool operator==(const SnmpTarget& lhs, const SnmpTarget& rhs)
{
  if (lhs.timeout_ != rhs.timeout_)
     return false;

  if (lhs.retries_ != rhs.retries_)
     return false;

  if (lhs.max_pdu_size_ != rhs.max_pdu_size_)
    return false;

  if (lhs.version_ != rhs.version_)
    return false;

  return true;
}


//----------------------------------------------------------------------
//--------[ IpTarget Member Functions ]----------------------------------
//----------------------------------------------------------------------


//---------[ UdpTarget::UdpTarget( void) ]----------------------------------

UdpTarget::UdpTarget()
{
  validity_ = 0;
  read_community_ = default_rd_community_; // assign by init fails
  write_community_ = default_wr_community_;
}

UdpTarget::UdpTarget(const UdpAddress& udp) : udp_address_(udp)
{
  if (udp_address_.valid())
    validity_ = 1;
  read_community_ = default_rd_community_;
  write_community_ = default_wr_community_;
}

// - copy constructor can be synthesised by compiler

//-----------[ UdpTarget::~UdpTarget() ]--------------------------------
UdpTarget::~UdpTarget()
{
}

//-----------[ UdpTarget::clone() ]--------------------------------
SnmpTarget * UdpTarget::clone() const
{
  return (SnmpTarget *) new UdpTarget(*this);
}


void UdpTarget::set_default_read_community(const OctetStr& rd_community)
{
   default_rd_community_ = rd_community;
}

void UdpTarget::set_default_write_community(const OctetStr& wr_community)
{
   default_wr_community_ = wr_community;
}


// get the read community name as an u_char and len
void UdpTarget::get_read_community( OctetStr& read_community_oct) const
{
  read_community_oct = read_community_;
}

//---------[ UdpTarget::set_getcommunity ]---------------------------------
// set the read community name
void UdpTarget::set_read_community( const OctetStr& new_read_community)
{
  read_community_ = new_read_community;
}

//---------[ UdpTarget::get_writecommunity ]----------------------------
// get the write community
void UdpTarget::get_write_community( OctetStr &write_community_oct) const
{
  write_community_oct = write_community_;
}

//-----------[ UdpTarget::set_writecommunity ]---------------------------
// set the write community
void UdpTarget::set_write_community( const OctetStr& write_community_oct)
{
  write_community_ = write_community_oct;
}

//------------[ Address& UdpTarget::get_address() ]---------------------
// get the address
void UdpTarget::get_address( UdpAddress &address) const
{
  address = udp_address_;
  return;
}

//-------------[ UdpTarget::set_address ]--------------------------------
// set the address
int UdpTarget::set_address( UdpAddress &udp_address)
{
  udp_address_ = udp_address;
  if ( udp_address_.valid())
    validity_ = 1;
  else
    validity_ = 0;

  return validity_;
}

// overloaded assignment
UdpTarget& UdpTarget::operator=( const UdpTarget& lhs)
{
  if (this == &lhs)
     return *this;

  // assign base part
  *((SnmpTarget *) this) = *((SnmpTarget *)&lhs);

  udp_address_ = lhs.udp_address_;
  read_community_ = lhs.read_community_;
  write_community_ = lhs.write_community_;
  validity_ = lhs.validity_;

  return *this;
}

const char *UdpTarget::to_string()
{
  ACE_OS::sprintf(output_buffer_,"UdpTarget: [ valid: %d addr: %s rd: %s wr: %s \
                  ver: %d, timeout: %d, retries: %d max_pdu_size: %d]",
                  valid(), udp_address_.to_string(), read_community_.to_string(),
                  write_community_.to_string(), version_, (int) timeout_,
                  retries_, max_pdu_size_);

  return output_buffer_;
}

//=============[ int operator == UdpTarget, UdpTarget ]===============
// equivlence operator overloaded
bool operator==( const UdpTarget &lhs,const UdpTarget &rhs)
{
  // need to compare all the members of a UdpTarget
  if ( lhs.read_community_ != rhs.read_community_)
    return false;  // !=

  if ( lhs.write_community_ != rhs.write_community_)
    return false;  // !=

  if ( lhs.udp_address_ != rhs.udp_address_)
    return false;

  if ( lhs.timeout_ != rhs.timeout_)
    return false;

  if ( lhs.retries_ != rhs.retries_)
    return false;

  return true;  // they are equal
}
