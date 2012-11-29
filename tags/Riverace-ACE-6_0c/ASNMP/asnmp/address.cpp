// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//     address.cpp
//
// = DESCRIPTION
//  The Vb class is an encapsulation of the snmp variable binding.
// This module contains the class definition for the variable binding (VB)
// class. The VB class is an encapsulation of a SNMP VB. A VB object is
// composed of one SNMP++ Oid and one SMI value. The Vb class utilizes Oid
// objects and thus requires the Oid class. To use this class,
// set oid, value then call valid() to be sure object was constructed correctly.
//
// = AUTHOR
//   Peter E Mellquist
//   Michael R MacFaden  mrm@cisco.com - rework & ACE port
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

#include "asnmp/address.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_Memory.h"

//=================================================================
//======== Abstract Address Class Implementation ==================
//=================================================================

// allow destruction of derived classes
Address::~Address()
{
}


//-----------------------------------------------------------------
// is the address object valid?
int Address::valid() const
{
  return valid_flag;
}

//------------[ Address::trim_white_space( char * ptr) ]------------
// destructive trim white space
void Address::trim_white_space( char * ptr)
{
  char *tmp;

  tmp = ptr;
  // skip leading white space
  while (*tmp==' ')tmp++;
  ACE_OS::strcpy(ptr,tmp);

  // find end of string
  while ((*tmp!=' ') && (*tmp !=0)) tmp++;
  if (*tmp!=0) *tmp=0;
}

//TM: this is not used nor needed, remove?
//-----[ element access ]----------------------------------------------
unsigned char& Address::operator[]( const int position)
{
  if ( position < MAX_ADDR_SZ)
    return  address_buffer[ position];
  else
    return address_buffer[0];
}


//-----------------------------------------------------------------------
// overloaded equivlence operator, are two addresses equal?
bool operator==( const Address &lhs, const Address &rhs)
{
  if ( ACE_OS::strcmp( (const char*) lhs, (const char*)rhs)==0)
    return true;
  else
    return false;
}

//-----------------------------------------------------------------------
// overloaded equivlence operator, are two addresses equal?
bool operator!=( const Address &lhs, const Address &rhs)
{
  return (!( lhs == rhs));
}


//------------------------------------------------------------------
// overloaded > operator, is a1 > a2
bool operator>( const Address &lhs, const Address &rhs)
{
  if (ACE_OS::strcmp( (const char*) lhs, (const char*)rhs)>0)
    return true;
  else
    return false;
}

// overloaded >= operator, is a1 > a2
bool operator>=( const Address &lhs,const Address &rhs)
{
  if (( lhs > rhs) || ( lhs == rhs))
    return true;
  else
    return false;
}

// overloaded < operator, is a1 <= a2
bool operator<=( const Address &lhs,const Address &rhs)
{
  if (( lhs < rhs) || ( lhs == rhs))
    return true;
  else
    return false;

}


//-----------------------------------------------------------------
// overloaded < operator, is a1 < a2
bool operator<( const Address &lhs, const Address &rhs)
{
  if (ACE_OS::strcmp( (const char*) lhs, (const char*)rhs)<0)
    return true;
  else
    return false;
}

//------------------------------------------------------------------
// equivlence operator overloaded, are an address and a string equal?
bool operator==( const Address &lhs,const char *rhs)
{
  if (!rhs && !lhs.valid())
    return true;
  if (ACE_OS::strcmp( (const char *) lhs, rhs)== 0)
    return true;
  else
    return false;
}

//------------------------------------------------------------------
// not equal operator overloaded, are an address and a string not equal?
bool operator!=( const Address &lhs,const char *rhs)
{
  return (!( lhs == rhs));
}

//------------------------------------------------------------------
// overloaded > , is a > inaddr
bool operator>( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return lhs.valid();  // if lhs valid then > 0, else invalid !> 0
  if (ACE_OS::strcmp( (const char *) lhs, rhs)> 0)
    return true;
  else
    return false;
}

//------------------------------------------------------------------
// overloaded >= , is a >= inaddr
bool operator>=( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return true; // always >= 0
  if (ACE_OS::strcmp( (const char *) lhs, rhs)>= 0)
    return true;
  else
    return false;
}

//-----------------------------------------------------------------
// overloaded < , are an address and a string equal?
bool operator<( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return false; // always >= 0
  if (ACE_OS::strcmp( (const char *) lhs, rhs)< 0)
    return true;
  else
    return false;
}

//-----------------------------------------------------------------
// overloaded <= , is a <= inaddr
bool operator<=( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return !lhs.valid(); // invalid == 0, else valid > 0
  if (ACE_OS::strcmp( (const char *) lhs, rhs) <= 0)
    return true;
  else
    return false;
}


//=====================================================================
//============ IPAddress Implementation ===============================
//=====================================================================

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 IpAddress::get_syntax()
{
  return sNMP_SYNTAX_IPADDR;
}

//-----[ IP Address copy constructor ]---------------------------------
IpAddress::IpAddress(const IpAddress &ipaddr)
  : Address (ipaddr)
{
  // always initialize what type this object is
  smival.syntax = sNMP_SYNTAX_IPADDR;
  smival.value.string.len =IPV4LEN;
  smival.value.string.ptr = address_buffer;

  iv_friendly_name_[0] = 0;
  iv_friendly_name_status_ = 0;
  valid_flag = ipaddr.valid_flag;
  if (valid_flag) {
    ACE_OS::memcpy(address_buffer, ipaddr.address_buffer,IPV4LEN);
    ACE_OS::strcpy( iv_friendly_name_, ipaddr.iv_friendly_name_);
  }

  IpAddress::format_output();
}


//-------[ default construct, an IP address with a string ]---------------------
IpAddress::IpAddress( const char *inaddr): Address()
{
  // always initialize what type this object is
  smival.syntax = sNMP_SYNTAX_IPADDR;
  smival.value.string.len = IPV4LEN;
  smival.value.string.ptr = address_buffer;

  if (ACE_OS::strlen(inaddr) == 0) {

    valid_flag = 0;
    iv_friendly_name_[0] = 0;
    iv_friendly_name_status_ = 0;
    IpAddress::format_output();
    return;
  }

  // parse_address initializes valid, address_buffer & iv_friendly_name_
  valid_flag = parse_address(inaddr);
  IpAddress::format_output();
}


//-----[ construct an IP address with a GenAddress ]---------------------
IpAddress::IpAddress( const GenAddress &genaddr)
{
  // always initialize what type this object is
  smival.syntax = sNMP_SYNTAX_IPADDR;
  smival.value.string.len = IPV4LEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  iv_friendly_name_[0] = 0;
  iv_friendly_name_status_ = 0;
  // allow use of an ip or udp genaddress
  if (genaddr.get_type() == type_ip) {
    valid_flag = genaddr.valid();
    if ( valid_flag) {
      // copy in the IP address data
      IpAddress temp_ip( (const char *) genaddr);
      *this = temp_ip;
    }
  }
  else
    if (genaddr.get_type() == type_udp) {
      valid_flag = genaddr.valid();
      if ( valid_flag) {
        // copy in the IP address data
        UdpAddress temp_udp( (const char *) genaddr);
        *this = temp_udp;
      }
    }
  IpAddress::format_output();
}

//-----[ destructor ]--------------------------------------------------
IpAddress::~IpAddress()
{
}

// assumes member data is word aligned to avoid sigbus
int IpAddress::is_loopback() const
{
 if (valid()) {
   u_long *pl = (u_long *)&address_buffer;
   return (*pl == INADDR_LOOPBACK);
 }
  return 0;
}

int IpAddress::is_multicast() const
{
 if (valid()) {
   u_long *pl = (u_long *)&address_buffer;
   return (IN_MULTICAST(*pl));
 }
  return 0;
}

// Private addressess not are not assignable in the Internet, they are
// defined in RFC 1597 as: 10, 172.16, and 192.168.0
// Some companies use them internally and apply NAT to allow translation
// instead of paying for ip networks.
// Cisco IOS devices can provide NAT aka Network Address Translation
// but don't expect SNMP based networks to handle cross-NAT address spaces.
// assumes storage in network byte order  mrm@cisco.com 7/28/97

int IpAddress::is_private() const
{
 if (valid()) {
   if (address_buffer[0] == 10)
     return 1;
   if (address_buffer[0] == 172 && address_buffer[1] == 16)
     return 1;
   if (address_buffer[0] == 192 && address_buffer[1] == 168  &&
      address_buffer[2] == 0)
     return 1;
 }
  return 0;
}


// convert address into octet string format in network byte order
void IpAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}


int IpAddress::is_broadcast() const
{
 if (valid()) {
   u_long *pl = (u_long *)&address_buffer;
   return ( (*pl) & INADDR_BROADCAST);
 }
  return 0;
}

//-----[ IP Address general = operator ]-------------------------------
SnmpSyntax& IpAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
    return *this;

  valid_flag = 0;       // will get set 1 if really valid
  iv_friendly_name_[0]=0;

  if (val.valid()) {
    switch (val.get_syntax()) {
    case sNMP_SYNTAX_IPADDR:
    case sNMP_SYNTAX_OCTETS:
      if (((IpAddress &)val).smival.value.string.len ==IPV4LEN) {
        ACE_OS::memcpy(address_buffer,
                       ((IpAddress &)val).smival.value.string.ptr,IPV4LEN);
        valid_flag = 1;
      }
      break;

      // NOTE: as a value add, other types could have "logical"
      // mappings, i.e. integer32 and unsigned32
    }
  }
  IpAddress::format_output();
  return *this;
}

//------[ assignment to another ipaddress object overloaded ]-----------------
IpAddress& IpAddress::operator=( const IpAddress &ipaddress)
{
  // protect against assignment from itself
  if ( this == &ipaddress )
    return *this;

  valid_flag = ipaddress.valid_flag;
  iv_friendly_name_[0]=0;

  if (valid_flag) {
    ACE_OS::memcpy(address_buffer, ipaddress.address_buffer, IPV4LEN);
    ACE_OS::strcpy(iv_friendly_name_, ipaddress.iv_friendly_name_);
  }
  IpAddress::format_output();
  return *this;
}


//--------[ create a new instance of this Value ]-----------------------
SnmpSyntax *IpAddress::clone() const
{
  return (SnmpSyntax *) new IpAddress(*this);
}

//-------[ return the Fully Qualified Domain Name ]----------------------
const char *IpAddress::resolve_hostname(int &status)
{
  if ((iv_friendly_name_[0] == 0) && valid_flag)
    addr_to_friendly();
  status = iv_friendly_name_status_;
  return iv_friendly_name_;
}

// parse a dotted string
int IpAddress::parse_dotted_ipstring( const char *inaddr)
{
  char *ip_token;
  int token_count=0;
  unsigned int value;
  int error_status = 0;
  char temp[MAXHOSTNAMELEN +1];  // temp buffer for destruction
  int z,w;

  // check len, an ip can never be bigger than 15
  // 123456789012345
  // XXX.XXX.XXX.XXX
  if ( !inaddr || (ACE_OS::strlen( inaddr) > 30))
    return 0;
  ACE_OS::strcpy( temp, inaddr);
  trim_white_space( temp);
  if ( ACE_OS::strlen( temp) > 15)
    return 0;

  // must only have three dots
  // strtok will not catch this !
  char *ptr = temp;
  int dot_count = 0;
  while ( *ptr != 0) {
    if ( *ptr == '.') dot_count++;
    ptr++;
  }
  if ( dot_count != 3)
    return 0;

  // look for dot token separator
  ip_token = ACE_OS::strtok( (char *) temp,".");

  // while more tokens..
  while ( ip_token != 0) {
    // verify that the token is all numerics
    w = ACE_OS::strlen( ip_token);
    if (w>3)
      return 0;
    for (z=0;z<w;z++)
      if (( ip_token[z] < '0') || ( ip_token[z] > '9'))
        return 0;

    value = ( unsigned int) ACE_OS::strtod(ip_token,0);
    if (( value > 0)&& ( value <=255))
      address_buffer[token_count] = (unsigned char) value;
    else
      if (ACE_OS::strcmp(ip_token,"0")==0)
        address_buffer[token_count]= (unsigned char) 0;
      else
        error_status = 1;
    token_count++;
    ip_token = ACE_OS::strtok( 0, ".");
  }

  // gota be four in len
  if ( token_count != 4)
    return 0;

  // any parsing errors?
  if ( error_status)
    return 0;

  return 1;
}

//-----[ IP Address parse Address ]---------------------------------
int IpAddress::parse_address( const char *inaddr)
{
  // parse the input char array
  // fill up internal buffer with four ip bytes
  // set and return validity flag

  in_addr ipAddr;
  char ds[MAXHOSTNAMELEN +1];

  // intialize the friendly_name member variable
  iv_friendly_name_[0] = 0;
  iv_friendly_name_status_ = 0;

  // is this a dotted IP notation string or
  // a friendly name
  if ( parse_dotted_ipstring( inaddr)) {

    // since this is a valid dotted string
    // don't do any DNS (Performance!)
    return 1;
  }
  else {
    int rc;
    if ((rc = resolve_to_address(inaddr, ipAddr)) == 0) {

        // now lets check out the dotted string
        ACE_OS::strncpy( ds, ACE_OS::inet_ntoa(ipAddr), MAXHOSTNAMELEN);

        if ( !parse_dotted_ipstring( ds))
          return 0;

        // save the friendly name
        ACE_OS::strcpy( iv_friendly_name_, inaddr);

    }    // end if lookup result
    else {
      iv_friendly_name_status_ = rc;
      return 0;
    }
  }  // end else not a dotted string
  return 1;
}

// using the currently defined address, do a DNS
// and try to fill up the name
int IpAddress::addr_to_friendly()
{
  in_addr ipAddr;
  long result = ACE_OS::inet_addr(to_string());
  if (result == -1)
    return -1;    // expected a dotted quad!

  ipAddr.s_addr = result;

  // set iv_friendly_name_ from ipAddr
 if (resolve_to_hostname(ipAddr, iv_friendly_name_) == 0) {
    return 0;
 }
 else {
#if defined (VXWORKS)
   // VxWorks doesn't have h_errno
   iv_friendly_name_status_ = errno;
#else
   iv_friendly_name_status_ = h_errno;
#endif /* VXWORKS */
   return iv_friendly_name_status_;
 }
}

// static aka class member function (1.2.3.4 -> foo.com)
int IpAddress::resolve_to_hostname(const in_addr& quad_addr, char *hostname)
{
  struct hostent lookupResult;
  int loc_errno = 0;
  ACE_HOSTENT_DATA buffer;
  ACE_OS::memset(&lookupResult, 0, sizeof(struct hostent));
  ACE_OS::memset(&buffer, 0, sizeof(ACE_HOSTENT_DATA));

  // reverse lookup (requires in-addr.arpa to be setup in DNS
  if (ACE_OS::gethostbyaddr_r((const char *)&quad_addr.s_addr, IPV4LEN,
                          AF_INET, &lookupResult, buffer, &loc_errno)) {

    // verify right type of record
    if (lookupResult.h_addrtype == AF_INET &&
        lookupResult.h_length == IPV4LEN) {
      ACE_OS::strcpy( hostname, lookupResult.h_name);
      // setup multiple entries
      return 0;
    }
    else {
      ACE_ASSERT(0);            // todo add trace and debug and dump
      return -1;                // wrong resource record type
    }
  }

  return loc_errno;
}

// static aka class member function (foo.com -> 1.2.3.4)
int IpAddress::resolve_to_address(const char *hostname, in_addr& quad_addr)
{
   struct hostent lookupResult;
   ACE_HOSTENT_DATA buffer;
   ACE_OS::memset(&buffer, 0, sizeof(ACE_HOSTENT_DATA));
   ACE_OS::memset(&lookupResult, 0, sizeof(struct hostent));
   int loc_errno = 0;
   if (ACE_OS::gethostbyname_r( hostname, &lookupResult, buffer, &loc_errno)) {
     if (lookupResult.h_length == sizeof(in_addr) &&
         lookupResult.h_addrtype == AF_INET) {
        ACE_OS::memcpy((void *) &quad_addr,
                       (void *) lookupResult.h_addr_list[0], sizeof(in_addr));
        return 0;
     }
    else
       return -1;  // wrong address size
   }
   return loc_errno;
}

//----[ IP address char * cast ]--------------------------------------
IpAddress::operator const char *() const
{
  return (char *)output_buffer;
}

//----[ IP address get char representation ]--------------------------
const char * IpAddress::to_string()
{
  return (char *)output_buffer;
}

//----[ IP address format output ]------------------------------------
void IpAddress::format_output()
{
  // if valid format else null it
  if ( valid_flag)
    ACE_OS::sprintf( (char *) output_buffer,"%d.%d.%d.%d",address_buffer[0],
             address_buffer[1], address_buffer[2], address_buffer[3]);
  else
    output_buffer[0] = 0;
}

//------[ return the type ]----------------------------------
addr_type IpAddress::get_type() const
{
  return type_ip;
}

//-----------------------------------------------------------------
// logically and two IPaddresses and
// return the new one
void IpAddress::mask( const IpAddress& ipaddr)
{
  if ( this->valid() && ipaddr.valid())  {
     this->address_buffer[0] = this->address_buffer[0] & ipaddr.address_buffer[0];
     this->address_buffer[1] = this->address_buffer[1] & ipaddr.address_buffer[1];
     this->address_buffer[2] = this->address_buffer[2] & ipaddr.address_buffer[2];
     this->address_buffer[3] = this->address_buffer[3] & ipaddr.address_buffer[3];
     format_output();
  }

}

//=======================================================================
//========== DNS Iterator Implementation ================================
//=======================================================================


Address_Iter::Address_Iter (const char *hostname)
  : valid_(0),
    count_(0),
    entry_(0)
{
  ACE_OS::memset (&buffer_, 0, sizeof (ACE_HOSTENT_DATA));
  ACE_OS::memset (&lookupResult_, 0, sizeof (struct hostent));

  if (ACE_OS::inet_addr (hostname) == (unsigned long) -1)
    {
      valid_ = query_dns (hostname);
    }
  else
    {
      ACE_ASSERT (0);             // don't support dot-quad lookup yet
    }

  // count number of hostnames
  int n;
  char **pc;

  for (n = 0, pc = lookupResult_.h_addr_list; *pc != 0; ++n, ++pc)
    {
      // Do nothing.
    }

  count_ = n;          // plus first one
  entry_ = lookupResult_.h_addr_list;
}

int Address_Iter::valid() const
{
  return (valid_ == 1);
}

int Address_Iter::how_many_addresses()
{
  return count_;
}

// return next entry, rc = 0, if entry is null return 0
int Address_Iter::next(IpAddress& addr)
{
  if (!entry_ || *entry_ == 0)
    return 1;

  IpAddress tmp(*entry_++); // return data
  addr = tmp;
  if (*entry_ == 0)
    return 1;
  return 0;
}

//  query DNS here
int Address_Iter::query_dns(const char *hostname)
{
  int loc_errno = 0;
  if (ACE_OS::gethostbyname_r( hostname, &lookupResult_, buffer_,
                                &loc_errno)) {
    if (lookupResult_.h_length == sizeof(IPV4LEN) &&
        lookupResult_.h_addrtype == AF_INET) {
      return 0;
    }
    else
      return -1;  // wrong address size
  }
  return loc_errno;
}

//=======================================================================
//========== Udp Address Implementation =================================
//=======================================================================

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 UdpAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

//-----------------[ construct an Udp address with another Udp address ]---
UdpAddress::UdpAddress( const UdpAddress  &udpaddr):IpAddress(udpaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = UDPIPV4LEN;
  smival.value.string.ptr = address_buffer;

  // Copy the port value
  set_port(udpaddr.get_port());
  format_output();
}

// default constructor with a dotted string
UdpAddress::UdpAddress( const char *inaddr):IpAddress()
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // Inherits IP Address attributes
    // Always initialize (override) what type this object is
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = UDPIPV4LEN;
    smival.value.string.ptr = address_buffer;
    set_port(0);
    format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = UDPIPV4LEN;
  smival.value.string.ptr = address_buffer;

   valid_flag = parse_address( (char *)inaddr);
   format_output();
}

//-----------------[ construct a UdpAddress from a GenAddress ]--------------
UdpAddress::UdpAddress( const GenAddress &genaddr):IpAddress()
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = UDPIPV4LEN;
  smival.value.string.ptr = address_buffer;

  unsigned int port = 0;
  valid_flag = 0;

  // allow use of an ip or udp genaddress
  if (genaddr.get_type() == type_udp)
  {
    valid_flag = genaddr.valid();
    if ( valid_flag)
    {
      // copy in the IP address data
      UdpAddress temp_udp( (const char *) genaddr);
      *this = temp_udp;

      //  port info since are making an UpAddress
      port = temp_udp.get_port();
    }
  }
  else
  if (genaddr.get_type() == type_ip)
  {
    valid_flag = genaddr.valid();
    if ( valid_flag)
    {
      // copy in the IP address data
      IpAddress temp_ip( (const char *) genaddr);
      *this = temp_ip;
    }
  }
  set_port(port);
  format_output();
}


//--------[ construct a udp from an IpAddress ]--------------------------
UdpAddress::UdpAddress( const IpAddress &ipaddr):IpAddress(ipaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = UDPIPV4LEN;
  smival.value.string.ptr = address_buffer;

  set_port(0);
  format_output();
}


//-----[ destructor ]--------------------------------------------------
UdpAddress::~UdpAddress()
{
}


  // copy an instance of this Value
SnmpSyntax& UdpAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag=0;         // will get set 1 if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_IPADDR:
    {
      UdpAddress temp_udp(val.to_string());
      *this = temp_udp; // valid_flag is set by the udp assignment
    }
    break;

    case sNMP_SYNTAX_OCTETS:
      if (((UdpAddress &)val).smival.value.string.len == UDPIPV4LEN){
         ACE_OS::memcpy(address_buffer,
                 ((UdpAddress &)val).smival.value.string.ptr, UDPIPV4LEN);
         iv_friendly_name_[0] = 0;
         valid_flag = 1;
      }
    break;

    // NOTE: as a value add, other types could have "logical"
    // mappings, i.e. integer32 and unsigned32
    }
  }
  format_output();
  return *this;
}

  // assignment to another UdpAddress object overloaded
UdpAddress& UdpAddress::operator=( const UdpAddress &udpaddr)
{
  // protect against assignment from itself
  if ( this == &udpaddr )
      return *this;

  (IpAddress &)*this = udpaddr; // use ancestor assignment for ipaddr value
  set_port(udpaddr.get_port()); // copy to port value
  format_output();
  return *this;
}


//-----[ IP Address parse Address ]---------------------------------
int UdpAddress::parse_address( const char *inaddr)
{
  char buffer[MAXHOSTNAMELEN +1];

  unsigned short port = 0;
  if (inaddr && (ACE_OS::strlen( inaddr)< MAXHOSTNAMELEN +1))
    ACE_OS::strcpy( buffer, inaddr);
  else {
    valid_flag = 0;
    return 0;
  }
  // look for port info @ the end of the string
  // port can be delineated by a ':' or a '/'
  // if neither are present then just treat it
  // like a normal IpAddress
  char *tmp;
  tmp = ACE_OS::strstr( buffer,":");
  if (tmp==0)
    tmp = ACE_OS::strstr(buffer,"/");

  if ( tmp != 0) {
    *tmp=0;   // null terminator
    tmp++;
    port = ACE_OS::atoi( tmp);
  }

  set_port(port);
  return IpAddress::parse_address( buffer);
}


//----------[ create a new instance of this Value ]------------------------
SnmpSyntax *UdpAddress::clone() const
{
  return (SnmpSyntax *) new UdpAddress(*this);
}

//--------[ set the port number ]---------------------------------------
void UdpAddress::set_port( const unsigned short p)
{
  unsigned short port_nbo = htons(p);
  ACE_OS::memcpy(&address_buffer[IPV4LEN], &port_nbo, 2);
  format_output();
}

//---------[ get the port number ]--------------------------------------
unsigned short UdpAddress::get_port() const
{
  if (valid_flag) {
    unsigned short port_nbo;
    ACE_OS::memcpy(&port_nbo, &address_buffer[IPV4LEN], 2);
    return ntohs(port_nbo);
  }
  else
    return 0;                   // don't use uninitialized memory
}

//------[ return the type ]--------------------------------------------
addr_type UdpAddress::get_type() const
{
  return type_udp;
}

//----[ UDP address char * cast ]--------------------------------------
UdpAddress::operator const char *() const
{
  return (char *)output_buffer;
}

//----[ UDP address get char representation ]--------------------------
const char * UdpAddress::to_string()
{
  return (char *)output_buffer;
}

// make format same as ACE_INET_Addr class (addr:port)
void UdpAddress::format_output()
{
  IpAddress::format_output(); // allow ancestors to format their buffers

  // if valid format else null it
  if ( valid_flag)
    ACE_OS::sprintf( (char *) output_buffer,"%s:%d",
                    IpAddress::to_string(),
                    get_port() );
  else
    output_buffer[0] = 0;
}


//=======================================================================
//=========== Netbios Address Implementation ============================
//=======================================================================

void NetbiosAddress::format_output()
{
  if ( valid_flag)
    ACE_OS::memcpy(output_buffer, address_buffer, NETBIOSLEN);
  else
    output_buffer[0] = 0;
}

void NetbiosAddress::InitNBAddr(const char *inaddr)
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = NETBIOSLEN;
    smival.value.string.ptr = address_buffer;

    valid_flag=0;
    NetbiosAddress::format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = parse_address( (char *) inaddr);
  NetbiosAddress::format_output();
}

// TODO: how to handle addresses < 15 chars (zero out or pad with ' ')
NetbiosAddress::NetbiosAddress( const char *inaddr)
{
  InitNBAddr(inaddr);

}

NetbiosAddress::NetbiosAddress( const char *inaddr,  nb_service svc)
{
  InitNBAddr(inaddr);
  address_buffer[15] = svc;
}

// TODO: go back over ms/ibm specs and verify this
// 16 chars, 15 can be any character, 16th is service number between 0 and 20
// names beginning with IBM are reserved, and hence invalid
// and doubt IBM would use this class anyway
int NetbiosAddress::parse_address(const char *address)
{
  if (ACE_OS::strlen(address) > NETBIOSLEN)
      return 0; // invalid

  if (ACE_OS::strncmp(address, "IBM", 3) == 0)
     return 0;   // invalid

  // addresses are free form but this  check may need to be expose to user
  //
  //if (address[15] < nb_workstation || address[15] > nb_server)
  //   return 0; //invalid service type

  ACE_OS::memset(address_buffer, 0, NETBIOSLEN);
  ACE_OS::memcpy(address_buffer, address, NETBIOSLEN);

  return 1; // valid
}

NetbiosAddress::NetbiosAddress( const NetbiosAddress& nbaddr)
  : Address (nbaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = NETBIOSLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = nbaddr.valid_flag;
  if (valid_flag)
     ACE_OS::memcpy(address_buffer, nbaddr.address_buffer, NETBIOSLEN);
  NetbiosAddress::format_output();
}

NetbiosAddress::NetbiosAddress( const GenAddress& genaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = NETBIOSLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  // allow use of an ipx or ipxsock address
  if ( (genaddr.get_type() == type_nb) ) {
    valid_flag = genaddr.valid();
    if ( valid_flag) {
      // copy in the Ipx address data
      NetbiosAddress temp_nb( (const char *) genaddr);
      *this = temp_nb;
    }
  }
  NetbiosAddress::format_output();
}

NetbiosAddress::~NetbiosAddress()
{
}

const char *NetbiosAddress::to_string()
{
  return (char *)output_buffer;
}

void NetbiosAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}

NetbiosAddress& NetbiosAddress::operator=( const NetbiosAddress &val)
{
 // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag = 0;         // will set to 1 if really valid
  if (val.valid()) {
    switch (((NetbiosAddress *) &val)->get_syntax()) {
    case sNMP_SYNTAX_OCTETS:
        ACE_OS::memcpy(address_buffer,
               ((NetbiosAddress &)val).smival.value.string.ptr, NETBIOSLEN);
        valid_flag = 1;
    break;
    }
  }
  NetbiosAddress::format_output();
  return *this;
}

nb_service NetbiosAddress::get_service_type() const
{
  return (nb_service) address_buffer[15];
}

void NetbiosAddress::set_service_type(nb_service nbservice)
{
  address_buffer[15] = nbservice;
  NetbiosAddress::format_output();
}

NetbiosAddress::operator const char *() const
{
  return (char *)output_buffer;
}

SnmpSyntax& NetbiosAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag = 0;         // will set to 1 if really valid
  if (val.valid()) {
    switch (val.get_syntax()) {
    case sNMP_SYNTAX_OCTETS:
      if (((NetbiosAddress &)val).smival.value.string.len == NETBIOSLEN) {
        ACE_OS::memcpy(address_buffer,
             ((NetbiosAddress &)val).smival.value.string.ptr, NETBIOSLEN);
        valid_flag=1;
      }
    break;
    }
  }
  NetbiosAddress::format_output();
  return *this;
}

SnmpSyntax *NetbiosAddress::clone() const
{
  return (SnmpSyntax *) new NetbiosAddress(*this);
}

SmiUINT32 NetbiosAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

addr_type NetbiosAddress::get_type() const
{
  return type_nb;
}

//=======================================================================
//=========== IPX Address Implementation ================================
//=======================================================================

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 IpxAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}


//----------[ default constructor with a string arg ]---------------------------
IpxAddress::IpxAddress( const char  *inaddr):Address( )
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = IPXLEN;
    smival.value.string.ptr = address_buffer;

    separator = '\0';
    valid_flag=0;
    IpxAddress::format_output();
    return;
  }
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXLEN;
  smival.value.string.ptr = address_buffer;

  separator = '\0';
  valid_flag = parse_address( (char *) inaddr);
  IpxAddress::format_output();
}


//-----[ IPX Address copy constructor ]----------------------------------
IpxAddress::IpxAddress(const IpxAddress &ipxaddr)
  : Address (ipxaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXLEN;
  smival.value.string.ptr = address_buffer;

  separator = '\0';
  valid_flag = ipxaddr.valid_flag;
  if (valid_flag)
     ACE_OS::memcpy(address_buffer, ipxaddr.address_buffer, IPXLEN);
  IpxAddress::format_output();
}


//----[ construct an IpxAddress from a GenAddress ]---------------------------
IpxAddress::IpxAddress( const GenAddress &genaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  // allow use of an ipx or ipxsock address
  if ( (genaddr.get_type() == type_ipx) ) {
    valid_flag = genaddr.valid();
    if ( valid_flag) {
      // copy in the Ipx address data
      IpxAddress temp_ipx( (const char *) genaddr);
      *this = temp_ipx;
    }
  }
  else
    if ( (genaddr.get_type() == type_ipxsock) ) {
      valid_flag = genaddr.valid();
      if ( valid_flag) {
        // copy in the Ipx address data
        IpxSockAddress temp_ipxsock( (const char *) genaddr);
        *this = temp_ipxsock;
      }
    }
  IpxAddress::format_output();
}


//-----[ destructor ]--------------------------------------------------
IpxAddress::~IpxAddress()
{
}


//-----[ IPX Address general = operator ]-------------------------------
SnmpSyntax& IpxAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag=0;         // will set to 1 if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_OCTETS:
      if (((IpxAddress &)val).smival.value.string.len == IPXLEN){
        ACE_OS::memcpy(address_buffer, ((IpxAddress &)val).smival.value.string.ptr, IPXLEN);
        valid_flag=1;
      }
    break;
    }
  }
  IpxAddress::format_output();
  return *this;
}

//--------[ assignment to another IpAddress object overloaded ]----------
IpxAddress& IpxAddress::operator=( const IpxAddress &ipxaddress)
{
  // protect against assignment from itself
  if ( this == &ipxaddress )
      return *this;

  valid_flag = ipxaddress.valid_flag;
  if (valid_flag)
    ACE_OS::memcpy(address_buffer, ipxaddress.address_buffer, IPXLEN);
  IpxAddress::format_output();
  return *this;
}


// create a new instance of this Value
SnmpSyntax *IpxAddress::clone() const
{
  return (SnmpSyntax *) new IpxAddress(*this);
}

//-----[ IPX Address parse Address ]-----------------------------------
// Convert a string to a ten byte ipx address
// On success sets validity  1 or 0
//
//     IPX address format
//
//  NETWORK ID| MAC ADDRESS
// 01 02 03 04|05 06 07 08 09 10
// XX XX XX XX|XX XX XX XX XX XX
//
//   Valid input format
//
//   XXXXXXXX.XXXXXXXXXXXX
//   Total length must be 21
//   Must have a separator in it
//   First string length must be 8
//   Second string length must be 12
//   Each char must take on value 0-F
//
//
// Input formats recognized
//
//  XXXXXXXX.XXXXXXXXXXXX
//  XXXXXXXX:XXXXXXXXXXXX
//  XXXXXXXX-XXXXXXXXXXXX
//  XXXXXXXX.XXXXXX-XXXXXX
//  XXXXXXXX:XXXXXX-XXXXXX
//  XXXXXXXX-XXXXXX-XXXXXX
int IpxAddress::parse_address( const char *inaddr)
{
  char unsigned *str1,*str2;
  char temp[30];    // don't destroy original
  char unsigned *tmp;
  size_t z, tmplen;


  // save the orginal source
  if (!inaddr || (ACE_OS::strlen( inaddr) >(sizeof(temp)-1))) return 0;
  ACE_OS::strcpy( temp, inaddr);
  trim_white_space( temp);
  tmplen = ACE_OS::strlen(temp);

  // bad total length check
  // 123456789012345678901
  // XXXXXXXX-XXXXXXXXXXXX  21 len
  //
  // XXXXXXXX-XXXXXX-XXXXXX 22 len
  // need at least 21 chars and no more than 22
  if ( (tmplen <21) || (tmplen >22))
    return 0;

  // convert the string to all lower case
  // this allows hex values to be in upper or lower
  for (z=0;z< tmplen;z++)
    temp[z] = ACE_OS::ace_tolower(temp[z]);

  // check for separated nodeid
  // if found remove it
  if (temp[15] == '-')
  {
     for(z=16;z<tmplen;z++)
        temp[z-1] = temp[z];
     temp[tmplen-1] = 0;
  }

  // no dot or colon separator check
  separator = temp[8];
  if (( separator != ':') &&
      ( separator != '.') &&
      ( separator != '-') &&
      ( separator != ' '))
    return 0;

  // separate the strings
  str1 = ( unsigned char *) temp;
  while( *str1 != separator) str1++;
  str2 = str1 + 1;
  *str1 = 0;
  str1= ( unsigned char *) temp;

  // check len of the network portion
  if ( ACE_OS::strlen((char *) str1) != 8)
     return 0;

  // check len of mac portion
  if ( ACE_OS::strlen( (char *) str2) != 12)
    return 0;

  // ok we like then lens, make sure that all chars are 0-f
  // check out the net id
  tmp = str1;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
        ((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return 0;

  // check out the MAC address
  tmp = str2;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
        ((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return 0;

  // convert to target string
  tmp = str1;
  while ( *tmp != 0)
  {
  if (( *tmp >= '0') && ( *tmp <= '9'))
    *tmp = *tmp - (char unsigned )'0';
  else
    *tmp = *tmp - (char unsigned) 'a' + (char unsigned) 10;
  tmp++;
  }

  // network id portion
  address_buffer[0] = (str1[0]*16) + str1[1];
  address_buffer[1] = (str1[2]*16) + str1[3];
  address_buffer[2] = (str1[4]*16) + str1[5];
  address_buffer[3] = (str1[6]*16) + str1[7];

  tmp = str2;
  while ( *tmp != 0)
  {
  if (( *tmp >= '0') && ( *tmp <= '9'))
    *tmp = *tmp - (char unsigned) '0';
  else
    *tmp = *tmp - (char unsigned) 'a'+ (char unsigned) 10;
  tmp++;
  }

  address_buffer[4] = (str2[0]*16)  + str2[1];
  address_buffer[5] = (str2[2]*16)  + str2[3];
  address_buffer[6] = (str2[4]*16)  + str2[5];
  address_buffer[7] = (str2[6]*16)  + str2[7];
  address_buffer[8] = (str2[8]*16)  + str2[9];
  address_buffer[9] = (str2[10]*16) + str2[11];

  return 1;
}

//----[ IPX address char * cast ]--------------------------------------
IpxAddress::operator const char *() const
{
  return (char *)output_buffer;
}

//----[ IPX address get char representation ]--------------------------
const char * IpxAddress::to_string()
{
  return (char *)output_buffer;
}


//----[ IPX address format output ]-------------------------------------
void IpxAddress::format_output()
{
  if ( valid_flag)
    ACE_OS::sprintf((char *) output_buffer,
            "%02x%02x%02x%02x%c%02x%02x%02x%02x%02x%02x",
            address_buffer[0],address_buffer[1],
            address_buffer[2],address_buffer[3],'-',
            address_buffer[4],address_buffer[5],
            address_buffer[6],address_buffer[7],
            address_buffer[8],address_buffer[9]);
  else
    output_buffer[0] = 0;
}


// get the host id portion of an ipx address
int IpxAddress::get_hostid( MacAddress& mac)
{
   if ( valid_flag)
   {
       char buffer[18];
       ACE_OS::sprintf( buffer,"%02x:%02x:%02x:%02x:%02x:%02x",
                address_buffer[4],
                address_buffer[5], address_buffer[6], address_buffer[7],
                address_buffer[8], address_buffer[9]);
       MacAddress temp( buffer);
       // mac = (SnmpSyntax&) temp;
       mac = temp;
       if ( mac.valid())
          return 1;
       else
          return 0;
   }
   else
      return 0;
}


//------[ return the type ]----------------------------------
addr_type IpxAddress::get_type() const
{
  return type_ipx;
}

void IpxAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}


//========================================================================
//======== IpxSockAddress Implementation =================================
//========================================================================

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 IpxSockAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

//-----------[ construct an IpxSockAddress with another IpxSockAddress]----
IpxSockAddress::IpxSockAddress( const IpxSockAddress &ipxaddr):IpxAddress(ipxaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXSOCKLEN;
  smival.value.string.ptr = address_buffer;

  // copy the socket value
  set_socket(ipxaddr.get_socket());
  format_output();
}


//---------------[ default construct a IpxSockAddress ]--------------
IpxSockAddress::IpxSockAddress( const char *inaddr):IpxAddress()
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = IPXSOCKLEN;
    smival.value.string.ptr = address_buffer;

    set_socket(0);
    format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXSOCKLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = parse_address( (char *) inaddr);
  format_output();
}


//---------------[ construct a IpxSockAddress from a GenAddress ]----------
IpxSockAddress::IpxSockAddress( const GenAddress &genaddr):IpxAddress()
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXSOCKLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  unsigned short socketid = 0;
  // allow use of an ipx or ipxsock address
  if ( (genaddr.get_type() == type_ipx) )
  {
    valid_flag = genaddr.valid();
    if ( valid_flag)
    {
      // copy in the Ipx address data
      IpxAddress temp_ipx( (const char *) genaddr);
      *this = temp_ipx;
    }
  }
  else
  if ( (genaddr.get_type() == type_ipxsock) )
  {
    valid_flag = genaddr.valid();
    if ( valid_flag)
    {
      // copy in the Ipx address data
      IpxSockAddress temp_ipxsock( (const char *) genaddr);
      *this = temp_ipxsock;
      //  socketid info since are making an IpxSockAddress
      socketid = temp_ipxsock.get_socket();
    }
  }
  set_socket(socketid);
  format_output();
}


//------------[ construct an IpxSockAddress from a IpxAddress ]--------------
IpxSockAddress::IpxSockAddress( const IpxAddress &ipxaddr):IpxAddress(ipxaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = IPXSOCKLEN;
  smival.value.string.ptr = address_buffer;

  set_socket(0);
  format_output();
}

//-----[ destructor ]--------------------------------------------------
IpxSockAddress::~IpxSockAddress()
{
}

// copy an instance of this Value
SnmpSyntax& IpxSockAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag=0;         // will set to 1 if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_OCTETS:
      {
        // See if it is of the Ipx address family
        // This handles IpxSockAddress == IpxAddress
        IpxSockAddress temp_ipx(val.to_string());
        if (temp_ipx.valid()){
          *this = temp_ipx;             // ipxsock = ipxsock
        }
        // See if it is an OctetStr of appropriate length
        else if (((IpxSockAddress &)val).smival.value.string.len == IPXSOCKLEN){
          ACE_OS::memcpy(address_buffer,
                 ((IpxSockAddress &)val).smival.value.string.ptr,
                 IPXSOCKLEN);
          valid_flag=1;
        }
      }
      break;
    }
  }
  format_output();
  return *this;
}

// assignment to another IpAddress object overloaded
IpxSockAddress& IpxSockAddress::operator=( const IpxSockAddress &ipxaddr)
{
  // protect against assignment from itself
  if ( this == &ipxaddr )
      return *this;

  (IpxAddress&)*this = ipxaddr;         // use ancestor assignment for ipx addr
  set_socket(ipxaddr.get_socket());     // copy socket value
  format_output();
  return *this;
}


//----------[ create a new instance of this Value ]------------------------
SnmpSyntax *IpxSockAddress::clone() const
{
  return (SnmpSyntax *) new IpxSockAddress(*this);
}

//----[ IPXSock address char * cast ]--------------------------------------
IpxSockAddress::operator const char *() const
{
  return (char *)output_buffer;
}

//----[ IPXSock address get char representation ]--------------------------
const char * IpxSockAddress::to_string()
{
  return (char *)output_buffer;
}

//----[ IPX address format output ]-------------------------------------
void IpxSockAddress::format_output()
{
  IpxAddress::format_output(); // allow ancestors to format their buffers

  if ( valid_flag)
    ACE_OS::sprintf((char *) output_buffer,"%s/%d",
            IpxAddress::to_string(), get_socket());
  else
    output_buffer[0] = 0;
}

//-----[ IP Address parse Address ]---------------------------------
int IpxSockAddress::parse_address( const char *inaddr)
{
   char buffer[MAXHOSTNAMELEN +1];
   unsigned short socketid=0;

   if (inaddr && (ACE_OS::strlen( inaddr)< MAXHOSTNAMELEN))
     ACE_OS::strcpy( buffer, inaddr);
   else
   {
     valid_flag = 0;
     return 0;
   }
   // look for port info @ the end of the string
   // port can be delineated by a ':' or a '/'
   // if neither are present then just treat it
   // like a normal IpAddress
   char *tmp;
   tmp = ACE_OS::strstr( buffer,"/");

   if (tmp != 0)
   {
     *tmp=0;   // null terminator
     tmp++;
     socketid = ACE_OS::atoi( tmp);
   }
   set_socket(socketid);
   return IpxAddress::parse_address( buffer);
}



//-------------[ set the socket number ]----------------------------------
void IpxSockAddress::set_socket( const unsigned short s)
{
  unsigned short sock_nbo = htons(s);
  ACE_OS::memcpy(&address_buffer[IPXLEN], &sock_nbo, 2);
}

//--------------[ get the socket number ]---------------------------------
unsigned short IpxSockAddress::get_socket() const
{
  if (valid_flag)
  {
    unsigned short sock_nbo;
    ACE_OS::memcpy(&sock_nbo, &address_buffer[IPXLEN], 2);
    return ntohs(sock_nbo);
  }
  else
    return 0; // don't use uninitialized memory
}

//------[ return the type ]----------------------------------------------
addr_type IpxSockAddress::get_type() const
{
  return type_ipxsock;
}


//========================================================================
//======== MACAddress Implementation =====================================
//========================================================================

//-----------[ syntax type ]----------------------------------------------
SmiUINT32 MacAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

//-----[ MAC Address copy constructor ]---------------------------------
MacAddress::MacAddress(const MacAddress &macaddr)
  : Address (macaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = MACLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = macaddr.valid_flag;
  if (valid_flag)
    ACE_OS::memcpy(address_buffer, macaddr.address_buffer, MACLEN);
  format_output();
}

//---------[ constructor with a string argument ]-------------------------
MacAddress::MacAddress( const char  *inaddr):Address( )
{
  if (ACE_OS::strlen(inaddr) == 0) {
     // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = MACLEN;
    smival.value.string.ptr = address_buffer;

    valid_flag=0;
    format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = MACLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = parse_address( (char *) inaddr);
  format_output();
}

//-----[ construct a MacAddress from a GenAddress ]------------------------
MacAddress::MacAddress( const GenAddress &genaddr)
{
  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = MACLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  // allow use of mac address
  if (genaddr.get_type() == type_mac)
  {
    valid_flag = genaddr.valid();
    if ( valid_flag)
    {
      // copy in the Mac address data
      MacAddress temp_mac( (const char *) genaddr);
      *this = temp_mac;
    }
  }
  format_output();
}

//-----[ destructor ]--------------------------------------------------
MacAddress::~MacAddress()
{
}

//---------[ MacAddress clone ]-------------------------------------------
SnmpSyntax *MacAddress::clone() const
{
  return (SnmpSyntax *) new MacAddress(*this);
}

//------[ assignment to another ipaddress object overloaded ]--------------
MacAddress& MacAddress::operator=( const MacAddress &macaddress)
{
  // protect against assignment from itself
  if ( this == &macaddress )
      return *this;

  valid_flag = macaddress.valid_flag;
  if (valid_flag)
    ACE_OS::memcpy(address_buffer, macaddress.address_buffer, MACLEN);
  format_output();
  return *this;
}



//-----[ MAC Address general = operator ]---------------------------------
SnmpSyntax& MacAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag=0;         // will set to 1 if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_OCTETS:
      if (((MacAddress &)val).smival.value.string.len == MACLEN){
        ACE_OS::memcpy(address_buffer,
        ((MacAddress &)val).smival.value.string.ptr, MACLEN);
        valid_flag=1;
      }
    break;
    }
  }
  format_output();
  return *this;
}

//-----[ MAC Address parse Address ]--------------------------------------
// Convert a string to a six byte MAC address
// On success sets validity 1 or 0
//
//     MAC address format
//
//   MAC ADDRESS
//   01 02 03 04 05 06
//   XX:XX:XX:XX:XX:XX
//   Valid input format
//
//   XXXXXXXXXXXX
//   Total length must be 17
//   Each char must take on value 0-F
//
//
int MacAddress::parse_address( const char *inaddr)
{
  char temp[MAXHOSTNAMELEN +1];    // don't destroy original
  char unsigned *tmp;
  size_t z;


  // save the orginal source
  if ( !inaddr || (ACE_OS::strlen( inaddr) > 30)) return 0;
  ACE_OS::strcpy( temp, inaddr);
  trim_white_space( temp);

  // bad total length check
  if ( ACE_OS::strlen(temp) != 17)
     return 0;

  // check for colons
  if ((temp[2] != ':')||(temp[5] != ':')||(temp[8]!=':')||(temp[11]!=':')
         ||(temp[14] !=':'))
     return 0;

  // strip off the colons
  tmp = ( unsigned char *) temp;
  int i = 0;
  while ( *tmp != 0)
  {
     if (*tmp != ':')
     {
        temp[i] = *tmp;
        i++;
     }
     tmp++;
  }
  temp[i] = 0;

  // convert to lower
  for(z=0;z<ACE_OS::strlen(temp);z++)
    temp[z] = ACE_OS::ace_tolower( temp[z]);


  // check out the MAC address
  tmp = ( unsigned char *) temp;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
        ((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return 0;

  // convert to target string
  tmp = (unsigned char *) temp;
  while ( *tmp != 0)
  {
  if (( *tmp >= '0') && ( *tmp <= '9'))
    *tmp = *tmp - (char unsigned )'0';
  else
    *tmp = *tmp - (char unsigned) 'a' + (char unsigned) 10;
  tmp++;
  }

  address_buffer[0] =  (temp[0]*16) + temp[1];
  address_buffer[1] =  (temp[2]*16) + temp[3];
  address_buffer[2] =  (temp[4]*16) + temp[5];
  address_buffer[3] =  (temp[6]*16) + temp[7];
  address_buffer[4] =  (temp[8]*16) + temp[9];
  address_buffer[5] =  (temp[10]*16) + temp[11];

  return 1;
}

//----[ MAC address char * cast ]--------------------------------------
MacAddress::operator const char *() const
{
  return (char *)output_buffer;
}
//----[ MAC address get char representation ]--------------------------
const char * MacAddress::to_string()
{
  return (char *)output_buffer;
}

//----[ MAC address format output ]---------------------------------
void MacAddress::format_output()
{
  if ( valid_flag)
    ACE_OS::sprintf(output_buffer,"%02x:%02x:%02x:%02x:%02x:%02x",address_buffer[0],
                    address_buffer[1],address_buffer[2],address_buffer[3],
                    address_buffer[4],address_buffer[5]);
  else
    output_buffer[0] = 0;
}

//------[ return the type ]----------------------------------
addr_type MacAddress::get_type() const
{
  return type_mac;
}


unsigned int MacAddress::hashFunction() const
{
        return ((((address_buffer[0] << 8) + address_buffer[1]) * HASH0LEN)
                + (((address_buffer[2] << 8) + address_buffer[3]) * HASH1LEN)
                + (((address_buffer[4] << 8) + address_buffer[5]) * HASH2LEN));
}

void MacAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}

//========================================================================
//========== Generic Address Implementation ==============================
//========================================================================

//-----------[ get the syntax]----------------------------------------------
SmiUINT32 GenAddress::get_syntax()
{
   if (address != 0)
       return address->get_syntax();

   return sNMP_SYNTAX_NULL;
}

void GenAddress::init_smi()
{
  smival.syntax = sNMP_SYNTAX_NULL;             // to be overridden
  smival.value.string.len = 0;                  // to be overridden
  smival.value.string.ptr = address_buffer;     // constant
}

//-----------[ constructor with a string argument ]----------------------
GenAddress::GenAddress( const char  *addr): address(0)
{
  valid_flag = 0;
  // initialize SMI info
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  init_smi();

  if (addr && ACE_OS::strlen(addr) == 0) {
    format_output();
    return;
  }

  parse_address(addr);

  // Copy real address smival info into GenAddr smival
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  if ( valid_flag ) {
      smival.syntax = ((GenAddress *)address)->smival.syntax;
      smival.value.string.len =
          ((GenAddress *)address)->smival.value.string.len;
      ACE_OS::memcpy(smival.value.string.ptr,
          ((GenAddress *)address)->smival.value.string.ptr,
          (size_t)smival.value.string.len);
  }
}

//-----------[ constructor with an Address argument ]--------------------
GenAddress::GenAddress( const Address &addr): address(0)
{
  valid_flag = 0;

  // initialize SMI info
  // BOK: this is generally not used for GenAddress,
  // but we need this to be a replica of the real address'
  // smival info so that operator=SnmpSyntax will work.
  init_smi();
  // make sure that the object is valid
  if (!addr.valid()) {
    format_output();
    return;
  }

  address = (Address*)addr.clone();
  if (address)
    valid_flag = address->valid();

  // Copy real address smival info into GenAddr smival
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  if ( valid_flag ) {
      smival.syntax = address->get_syntax();
      smival.value.string.len =
          ((GenAddress *)address)->smival.value.string.len;
      ACE_OS::memcpy(smival.value.string.ptr,
          ((GenAddress *)address)->smival.value.string.ptr,
          (size_t)smival.value.string.len);
  }

  format_output();
}

//-----------------[ constructor with another GenAddress object ]-------------
GenAddress::GenAddress( const GenAddress &addr)
  : Address (addr),
    address(0)
{
  valid_flag = 0;

  // initialize SMI info
  // BOK: this is generally not used for GenAddress,
  // but we need this to be a replica of the real address'
  // smival info so that operator=SnmpSyntax will work.
  init_smi();

  // make sure that the object is valid
  if (!addr.valid_flag) {
    format_output();
    return;
  }

  address = (Address *)addr.address->clone();
  if (address)
    valid_flag = address->valid();

  // Copy real address smival info into GenAddr smival
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  if ( valid_flag ) {
      smival.syntax = ((GenAddress *)address)->smival.syntax;
      smival.value.string.len =
          ((GenAddress *)address)->smival.value.string.len;
      ACE_OS::memcpy(smival.value.string.ptr,
          ((GenAddress *)address)->smival.value.string.ptr,
          (size_t)smival.value.string.len);
  }

  format_output();
}

//----------[ destructor ] ------------------------------------------------
GenAddress::~GenAddress()
{
  if ( address != 0)
    delete address;
}

//----------[ create a new instance of this Value ]------------------------
SnmpSyntax *GenAddress::clone() const
{
  return (SnmpSyntax *) new GenAddress(*this);
}

//------[ assignment GenAddress = GenAddress ]-----------------------------
GenAddress& GenAddress::operator=( const GenAddress &addr)
{
  // protect against assignment from itself
  if ( this == &addr )
      return *this;

  valid_flag = 0;
  if (address) {
    delete address;
    address = 0;
  }

  if (addr.address)
    address = (Address *)(addr.address)->clone();

  if (address)
    valid_flag = address->valid();

  // Copy real address smival info into GenAddr smival
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  if ( valid_flag ) {
      smival.syntax = ((GenAddress *)address)->smival.syntax;
      smival.value.string.len =
          ((GenAddress *)address)->smival.value.string.len;
      ACE_OS::memcpy(smival.value.string.ptr,
          ((GenAddress *)address)->smival.value.string.ptr,
          (size_t)smival.value.string.len);
  }

  format_output();
  return *this;
}


//------[ assignment GenAddress = any SnmpSyntax ]-----------------------
SnmpSyntax& GenAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;

  valid_flag = 0;               // will get set to 1 if really valid
  if ( address != 0) {
    delete address;
    address = 0;
  }

  if (val.valid())  {
    switch ( val.get_syntax() ) {
      //-----[ ip address case ]-------------
      // BOK: this case shouldn't be needed since there is an explicit
      // GenAddr=Address assignment that will override this assignment.
      // Left here for posterity.
    case sNMP_SYNTAX_IPADDR:
    {
      address = (Address *)val.clone();
      if (address)
        valid_flag = address->valid();
    }
    break;

      //-----[ udp address case ]------------
      //-----[ ipx address case ]------------
      //-----[ mac address case ]------------
      // BOK:  This is here only to support GenAddr = primitive OctetStr.
      // The explicit GenAddr=Address assignment will handle the cases
      // GenAddr = [UdpAdd|IpxAddr|IpxSock|MacAddr|DecNetAddr|NetbiosAddr|AppleTlk].
      // Note, using the heuristic of octet str len to determine type of
      // address to create is not accurate when address lengths are equal
      // (e.g., UDPIPV4LEN == MACLEN).  It gets worse if we add AppleTalk or
      // OSI which use variable length addresses! assume AppleTalk as used in IOS Mibs
      // is defined in CISCO-TC.my as fixed length of 3 octets

    case sNMP_SYNTAX_OCTETS:
    {
      unsigned long val_len;
      val_len = ((GenAddress &)val).smival.value.string.len;

      if (val_len == UDPIPV4LEN) {
        ACE_NEW_RETURN(address,  UdpAddress, *this);
      }
      else if (val_len ==IPV4LEN) {
        ACE_NEW_RETURN(address, IpAddress, *this);
      }
      else if (val_len == IPXLEN) {
        ACE_NEW_RETURN(address, IpxAddress, *this);
      }
      else if (val_len == IPXSOCKLEN) {
        ACE_NEW_RETURN(address, IpxSockAddress, *this);
      }
      else  if (val_len == MACLEN) {
        ACE_NEW_RETURN(address, MacAddress, *this);
      }
      else if (val_len == APPLETKLEN) {
        ACE_NEW_RETURN(address, AppleTalkAddress, *this);
      }
      else if (val_len == DECNETLEN) {
        ACE_NEW_RETURN(address, DecNetAddress, *this);
      }
      else if (val_len == NETBIOSLEN) {
        ACE_NEW_RETURN(address, DecNetAddress, *this);
      }

      if (address) {
        *address = val;
        valid_flag = address->valid();
      }
    }
    break;
    }   // end switch
  }

  // Copy real address smival info into GenAddr smival
  // BOK: smival is generally not used for GenAddress, but
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax
  //      will work.
  if ( valid_flag ) {
      smival.syntax = ((GenAddress *)address)->smival.syntax;
      smival.value.string.len =
          ((GenAddress *)address)->smival.value.string.len;
      ACE_OS::memcpy(smival.value.string.ptr,
          ((GenAddress *)address)->smival.value.string.ptr,
          (size_t)smival.value.string.len);
  }

  format_output();
  return *this;
}


// redefined parse address for macs
// TODO: Add netbios, appletalk, and decnet addresses here
int GenAddress::parse_address( const char *addr)
{
   if ( address != 0)
      delete address;

   // try to create each of the addresses until the correct one
   // is found

//BOK: Need to try IPX Sock and IPX before UDP since on Win32,
//     gethostbyname() seems to think the ipx network number
//     portion is a valid ipaddress string... stupid WinSOCK!

    // ipxsock address
    ACE_NEW_RETURN(address, IpxSockAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag && ((IpxSockAddress*)address)->get_socket()) {
       format_output();
       return 1;   // ok its an ipxsock address
    }
        // otherwise delete it and try another
    delete address;

    // ipx address
    ACE_NEW_RETURN(address, IpxAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag)  {
       format_output();
       return 1;   // ok its an ipx address
    }
        // otherwise delete it and try another
    delete address;

//TM: Must try the derived classes first...one pitfall of the
//following solution is if someone creates with a port/socket of 0 the
//class will get demoted to ip/ipx.  The only proper way to do this is
//to parse the strings ourselves.

    // udp address
    ACE_NEW_RETURN(address, UdpAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag && ((UdpAddress*)address)->get_port()) {
       format_output();
       return 1;       // ok its a udp address
    }

    // otherwise delete it and try another
    delete address;

    // ip address
    ACE_NEW_RETURN(address, IpAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag)
    {
       format_output();
       return 1;       // ok its an ip address
    }
        // otherwise delete it and try another
    delete address;

    // mac address
    ACE_NEW_RETURN(address, MacAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag) {
       format_output();
       return 1;    // ok, its a mac
    }

    // guess by length of argument the type of address
    switch  (ACE_OS::strlen(addr)) {
    case NETBIOSLEN:
        ACE_NEW_RETURN(address, NetbiosAddress( addr), -1);
        valid_flag = address->valid();
        if ( valid_flag) {
          format_output();
          return 1;    // ok, its a mac
        }
        break;

    case APPLETKLEN:
        ACE_NEW_RETURN(address, AppleTalkAddress( addr), -1);
        valid_flag = address->valid();
        if ( valid_flag) {
          format_output();
          return 1;    // ok, its a mac
        }
        break;

    case DECNETLEN:
        ACE_NEW_RETURN(address, DecNetAddress( addr), -1);
        valid_flag = address->valid();
        if ( valid_flag) {
          format_output();
          return 1;    // ok, its a mac
        }
        break;

    }
        // otherwise its invalid
    delete address;
    address = 0;
    format_output();
    return 0;
}

GenAddress::operator const char *() const
{
  if ( address != 0)
    return (const char *)*address;      // pass thru
  else
    return (char *)output_buffer;
}

// to_string form of the contained address
const char * GenAddress::to_string()
{
  if ( address != 0)
    return address->to_string();        // pass thru
  else
    return (char *)output_buffer;
}

// format output
void GenAddress::format_output()
{
  output_buffer[0] = '\0';
}

//------[ return the type ]----------------------------------
addr_type GenAddress::get_type() const
{
  if (!valid())
    return type_invalid;
  else
    return address->get_type();
}

// call the particular type class here
void GenAddress::to_octet(OctetStr& octet) const
{
  if (!valid())
    return;

  address->to_octet(octet);
}

//------------------------------------------------------------------------
//---------[ DecNet Address Class ]---------------------------------------
//------------------------------------------------------------------------

DecNetAddress::DecNetAddress( const char *inaddr): Address()
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = DECNETLEN;
    smival.value.string.ptr = address_buffer;

    valid_flag=0;
    DecNetAddress::format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = DECNETLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = parse_address( (char *) inaddr);
  DecNetAddress::format_output();
}

DecNetAddress::DecNetAddress( const DecNetAddress& addr)
  : Address (addr)
{
}

DecNetAddress::DecNetAddress( const GenAddress& genaddr)
{
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = DECNETLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  // allow use of an ipx or ipxsock address
  if ( (genaddr.get_type() == type_decnet) ) {
    valid_flag = genaddr.valid();
    if ( valid_flag) {
      // copy in the Ipx address data
      DecNetAddress temp_ipx( (const char *) genaddr);
      *this = temp_ipx;
    }
  }
  DecNetAddress::format_output();
}

DecNetAddress::~DecNetAddress()
{
}

const char *DecNetAddress::to_string()
{
  return (char *)output_buffer;
}

DecNetAddress& DecNetAddress::operator=( const DecNetAddress &decaddr)
{
  // protect against assignment from itself
  if ( this == &decaddr )
      return *this;
  valid_flag = decaddr.valid_flag;
  if (valid_flag)
    ACE_OS::memcpy(address_buffer, decaddr.address_buffer, DECNETLEN);
  format_output();
  return *this;
}

void DecNetAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}

DecNetAddress::operator const char *() const
{
  return (char *)output_buffer;
}

SmiUINT32 DecNetAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

SnmpSyntax& DecNetAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
    return *this;

  valid_flag = 0;       // will get set 1 if really valid

  if (val.valid()) {
      if (((DecNetAddress &)val).smival.value.string.len ==DECNETLEN) {
        ACE_OS::memcpy(address_buffer,
                   ((DecNetAddress &)val).smival.value.string.ptr, DECNETLEN);
        valid_flag = 1;
      }
  }
  DecNetAddress::format_output();
  return *this;
}

SnmpSyntax *DecNetAddress::clone() const
{
  return (SnmpSyntax *) new DecNetAddress(*this);
}

addr_type DecNetAddress::get_type() const
{
  return type_decnet;
}

void DecNetAddress::format_output()
{
  // if valid format else null it
  if ( valid_flag)
    ACE_OS::sprintf( (char *) output_buffer,"%d.%d",address_buffer[0],
             address_buffer[1]);
  else
    output_buffer[0] = 0;
}

int DecNetAddress::parse_address( const char *address)
{
 if (ACE_OS::strlen(address) > DECNETLEN)
      return 0; // invalid

  ACE_OS::memset(address_buffer, 0, DECNETLEN);
  ACE_OS::memcpy(address_buffer, address, DECNETLEN);

  return 1; // valid
}


//------------------------------------------------------------------------
//---------[ AppleTalk Address Class ]------------------------------------
//------------------------------------------------------------------------

AppleTalkAddress::AppleTalkAddress( const char *inaddr): Address()
{
  if (ACE_OS::strlen(inaddr) == 0) {
    // always initialize SMI info
    smival.syntax = sNMP_SYNTAX_OCTETS;
    smival.value.string.len = APPLETKLEN;
    smival.value.string.ptr = address_buffer;

    valid_flag=0;
    AppleTalkAddress::format_output();
    return;
  }

  // always initialize SMI info
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = APPLETKLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = parse_address( (char *) inaddr);
  AppleTalkAddress::format_output();
}
AppleTalkAddress::AppleTalkAddress( const AppleTalkAddress& addr)
  : Address (addr)
{
}

AppleTalkAddress::AppleTalkAddress( const GenAddress& genaddr)
{
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = APPLETKLEN;
  smival.value.string.ptr = address_buffer;

  valid_flag = 0;
  // allow use of an ipx or ipxsock address
  if ( (genaddr.get_type() == type_atk) ) {
    valid_flag = genaddr.valid();
    if ( valid_flag) {
      // copy in the Ipx address data
      AppleTalkAddress temp_ipx( (const char *) genaddr);
      *this = temp_ipx;
    }
  }
  AppleTalkAddress::format_output();
}

AppleTalkAddress::~AppleTalkAddress()
{
}

const char *AppleTalkAddress::to_string()
{
  return (char *)output_buffer;
}

AppleTalkAddress& AppleTalkAddress::operator=( const AppleTalkAddress &ataddr)
{
  // protect against assignment from itself
  if ( this == &ataddr )
      return *this;
  valid_flag = ataddr.valid_flag;
  if (valid_flag)
    ACE_OS::memcpy(address_buffer, ataddr.address_buffer, APPLETKLEN);
  format_output();
  return *this;
}

void AppleTalkAddress::to_octet(OctetStr& octet) const
{
   octet.set_data( smival.value.string.ptr, smival.value.string.len);
}

AppleTalkAddress::operator const char *() const
{
  return (char *)output_buffer;
}

SmiUINT32 AppleTalkAddress::get_syntax()
{
  return sNMP_SYNTAX_OCTETS;
}

SnmpSyntax& AppleTalkAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
    return *this;

  valid_flag = 0;       // will get set 1 if really valid

  if (val.valid()) {
      if (((AppleTalkAddress &)val).smival.value.string.len ==APPLETKLEN) {
        ACE_OS::memcpy(address_buffer,
                   ((AppleTalkAddress &)val).smival.value.string.ptr,APPLETKLEN);
        valid_flag = 1;
      }
  }
  AppleTalkAddress::format_output();
  return *this;
}

SnmpSyntax *AppleTalkAddress::clone() const
{
  return (SnmpSyntax *) new AppleTalkAddress(*this);
}

addr_type AppleTalkAddress::get_type() const
{
  return type_atk;
}

void AppleTalkAddress::format_output()
{
  // if valid format else null it
  if ( valid_flag)
    ACE_OS::sprintf( (char *) output_buffer,"%d.%d.%d", address_buffer[0],
             address_buffer[1], address_buffer[3]);
  else
    output_buffer[0] = 0;
}

int AppleTalkAddress::parse_address( const char *address)
{
 if (ACE_OS::strlen(address) > APPLETKLEN)
      return 0; // invalid

  ACE_OS::memset(address_buffer, 0, APPLETKLEN);
  ACE_OS::memcpy(address_buffer, address, APPLETKLEN);

  return 1; // valid
}

char AppleTalkAddress::get_host_address() const
{
  return address_buffer[2];
}

void AppleTalkAddress::set_host_address(const char host)
{
  address_buffer[2] = host;
}

short AppleTalkAddress::get_net_address() const
{
  short net;
  ACE_OS::memcpy(&net, address_buffer, APPLETKLEN - 1);
  return net;
}

void AppleTalkAddress::set_net_address(const short atknet)
{
  ACE_OS::memcpy(address_buffer, &atknet, APPLETKLEN -1);
}
