/* -*-C++-*- */
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
int operator==( const Address &lhs, const Address &rhs)
{
  if ( strcmp( (const char*) lhs, (const char*)rhs)==0)
    return TRUE;
  else
    return FALSE;
}

//-----------------------------------------------------------------------
// overloaded equivlence operator, are two addresses equal?
int operator!=( const Address &lhs, const Address &rhs)
{
  return (!( lhs == rhs));
}


//------------------------------------------------------------------
// overloaded > operator, is a1 > a2
int operator>( const Address &lhs, const Address &rhs)
{
  if ( strcmp( (const char*) lhs, (const char*)rhs)>0)
    return TRUE;
  else
    return FALSE;
}

// overloaded >= operator, is a1 > a2
int operator>=( const Address &lhs,const Address &rhs)
{
  if (( lhs > rhs) || ( lhs == rhs))
    return TRUE;
  else
    return FALSE;
}

// overloaded < operator, is a1 <= a2
int operator<=( const Address &lhs,const Address &rhs)
{
  if (( lhs < rhs) || ( lhs == rhs))
    return TRUE;
  else
    return FALSE;

}


//-----------------------------------------------------------------
// overloaded < operator, is a1 < a2
int operator<( const Address &lhs, const Address &rhs)
{
  if ( strcmp( (const char*) lhs, (const char*)rhs)<0)
    return TRUE;
  else
    return FALSE;
}

//------------------------------------------------------------------
// equivlence operator overloaded, are an address and a string equal?
int operator==( const Address &lhs,const char *rhs)
{
  if (!rhs && !lhs.valid())
    return TRUE;
  if (strcmp( (const char *) lhs, rhs)== 0)
    return TRUE;
  else
    return FALSE;
}

//------------------------------------------------------------------
// not equal operator overloaded, are an address and a string not equal?
int operator!=( const Address &lhs,const char *rhs)
{
  return (!( lhs == rhs));
}

//------------------------------------------------------------------
// overloaded > , is a > inaddr
int operator>( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return lhs.valid();  // if lhs valid then > NULL, else invalid !> NULL
  if (strcmp( (const char *) lhs, rhs)> 0)
    return TRUE;
  else
    return FALSE;
}

//------------------------------------------------------------------
// overloaded >= , is a >= inaddr
int operator>=( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return TRUE; // always >= NULL
  if (strcmp( (const char *) lhs, rhs)>= 0)
    return TRUE;
  else
    return FALSE;
}

//-----------------------------------------------------------------
// overloaded < , are an address and a string equal?
int operator<( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return FALSE; // always >= NULL
  if (strcmp( (const char *) lhs, rhs)< 0)
    return TRUE;
  else
    return FALSE;
}

//-----------------------------------------------------------------
// overloaded <= , is a <= inaddr
int operator<=( const Address &lhs,const char *rhs)
{
  if (!rhs)
    return !lhs.valid(); // invalid == NULL, else valid > NULL
  if (strcmp( (const char *) lhs, rhs)<= 0)
    return TRUE;
  else
    return FALSE;
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
{
  // always initialize what type this object is
  smival.syntax = sNMP_SYNTAX_IPADDR;
  smival.value.string.len =IPV4LEN;
  smival.value.string.ptr = address_buffer;

  iv_friendly_name[0]=0;
  iv_friendly_name_status=0;  
  valid_flag = ipaddr.valid_flag; 
  if (valid_flag) {
    // copy the address data    
    ACE_OS::memcpy(address_buffer, ipaddr.address_buffer,IPV4LEN);
    // and the friendly name
    ACE_OS::strcpy( iv_friendly_name, ipaddr.iv_friendly_name);
  }

  IpAddress::format_output();
}


//-------[ default construct, an IP address with a string ]---------------------
IpAddress::IpAddress( const char *inaddr):Address()
{ 
  // always initialize what type this object is
  smival.syntax = sNMP_SYNTAX_IPADDR;
  smival.value.string.len = IPV4LEN;
  smival.value.string.ptr = address_buffer;

  if (ACE_OS::strlen(inaddr) == 0) {

    valid_flag = FALSE; 
    iv_friendly_name[0]=0; 
    iv_friendly_name_status=0; 
    IpAddress::format_output();
    return;
  }

  // parse_address initializes valid, address_buffer & iv_friendly_name
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

  valid_flag = FALSE;
  iv_friendly_name[0] = 0;
  iv_friendly_name_status = 0;
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
   return (*(u_long *)&address_buffer == INADDR_LOOPBACK);
 }
  return FALSE; 
}

int IpAddress::is_multicast() const
{
 if (valid()) {
   return (IN_MULTICAST(*(u_long *)&address_buffer));
 }
  return FALSE; 
}

// The old ARPA net used 10.x.x.x. But now it is not assignable in the Internet 
// even though it is no longer used. Some companies use it 
// internally and apply NAT to allow translation instead of paying for 
// ip networks. Cisco IOS can provide NAT or Network Address Translation
// but don't expect SNMP based networks to handle cross-NAT address spaces. 
// assumes storage in network byte order  mrm@cisco.com 7/28/97

int IpAddress::is_arpanet() const
{
 if (valid()) {
   return (address_buffer[0] == 10);
 }
  return FALSE; 
}

int IpAddress::is_broadcast() const
{
 if (valid()) {
   return ( (*(u_long *)&address_buffer) & INADDR_BROADCAST);
 }
  return FALSE; 
}

//-----[ IP Address general = operator ]-------------------------------
SnmpSyntax& IpAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
    return *this;

  valid_flag = 0;	// will get set TRUE if really valid
  iv_friendly_name[0]=0;  

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
  iv_friendly_name[0]=0;  

  if (valid_flag) {
    ACE_OS::memcpy(address_buffer, ipaddress.address_buffer, IPV4LEN);
    ACE_OS::strcpy(iv_friendly_name, ipaddress.iv_friendly_name);
  }
  IpAddress::format_output();
  return *this;
}


//--------[ create a new instance of this Value ]-----------------------
SnmpSyntax *IpAddress::clone() const 
{ 
  return (SnmpSyntax *) new IpAddress(*this); 
}

//-------[ return the friendly name ]----------------------------------
char *IpAddress::friendly_name(int &status)
{  
  if ((iv_friendly_name[0]==0) && (valid_flag))
    this->addr_to_friendly();
  status = iv_friendly_name_status;
  return iv_friendly_name; 
}

// parse a dotted string
int IpAddress::parse_dotted_ipstring( const char *inaddr)
{
  char *ip_token;
  int token_count=0;
  unsigned int value;
  int error_status = FALSE;
  char temp[MAXHOSTNAMELEN +1];  // temp buffer for destruction
  int z,w;

  // check len, an ip can never be bigger than 15
  // 123456789012345
  // XXX.XXX.XXX.XXX
  if ( !inaddr || (ACE_OS::strlen( inaddr) > 30)) 
    return FALSE;
  ACE_OS::strcpy( temp, inaddr);
  trim_white_space( temp);
  if ( ACE_OS::strlen( temp) > 15) 
    return FALSE;

  // must only have three dots
  // strtok will not catch this !
  char *ptr = temp;
  int dot_count = 0;
  while ( *ptr != 0) {
    if ( *ptr == '.') dot_count++;
    ptr++;
  }
  if ( dot_count != 3)
    return FALSE;

  // look for dot token separator
  ip_token = strtok( (char *) temp,".");

  // while more tokens..
  while ( ip_token != NULL) {
    // verify that the token is all numerics
    w = ACE_OS::strlen( ip_token);
    if (w>3) 
      return FALSE;
    for (z=0;z<w;z++)
      if (( ip_token[z] < '0') || ( ip_token[z] > '9'))
	return FALSE;

    value = ( unsigned int) strtod(ip_token,NULL);
    if (( value > 0)&& ( value <=255))
      address_buffer[token_count] = (unsigned char) value;
    else
      if ( strcmp(ip_token,"0")==0)
	address_buffer[token_count]= (unsigned char) 0;
      else
	error_status = TRUE;
    token_count++;
    ip_token = strtok( NULL, ".");
  }

  // gota be four in len
  if ( token_count != 4)
    return FALSE;

  // any parsing errors?
  if ( error_status)
    return FALSE;

  return TRUE;
}

//-----[ IP Address parse Address ]---------------------------------
int IpAddress::parse_address( const char *inaddr)
{
  // parse the input char array
  // fill up internal buffer with four ip bytes
  // set and return validity flag

  in_addr ipAddr;
  hostent *lookupResult;
  char	   *namePtr = NULL;
  char ds[MAXHOSTNAMELEN +1];


  // intialize the friendly_name member variable
  iv_friendly_name[0] = 0;
  iv_friendly_name_status = 0;

  // is this a dotted IP notation string or
  // a friendly name
  if ( parse_dotted_ipstring( inaddr)) {

    // since this is a valid dotted string
    // don't do any DNS
    return TRUE;
  }
  else {
    // not a dotted string, try to resolve it via DNS
    // if its an ipx or a mac then get out 
    // and don't do DNS
    IpxAddress ipxtest( inaddr);
    if ( ipxtest.valid())
      return FALSE;

    MacAddress mactest( inaddr);
    if ( mactest.valid())
      return FALSE;

    int rc;
    if ((rc = resolve_to_address(inaddr, ipAddr)) == 0) {

	// now lets check out the dotted string
	ACE_OS::strncpy( ds, ACE_OS::inet_ntoa(ipAddr), MAXHOSTNAMELEN);

	if ( !parse_dotted_ipstring( ds))
	  return FALSE;

	// save the friendly name
	ACE_OS::strcpy( iv_friendly_name, inaddr);
	return TRUE;

    }	 // end if lookup result
    else {
      iv_friendly_name_status = rc;
      return FALSE;
    }
  }  // end else not a dotted string
  return TRUE;
}

// using the currently defined address, do a DNS
// and try to fill up the name
int IpAddress::addr_to_friendly()
{
  in_addr ipAddr;
  char  *namePtr = NULL;
  char  ds[MAXHOSTNAMELEN + 1];

  // can't look up an invalid address
  if ( !valid_flag) 
     return -1;

 // otherwise lets look it up
 // lets try and get the friendly name
 // from the DNS
 ACE_OS::strncpy( ds, IpAddress::to_string(), MAXHOSTNAMELEN);

 if ((ipAddr.s_addr = ACE_OS::inet_addr((char *) ds)) == -1)
   return -1;    // bad address

 // if we found the name, then update the
 // iv friendly name
 if (resolve_to_hostname(ipAddr, iv_friendly_name) == 0) {
    return 0;
 }
 else {
   iv_friendly_name_status = h_errno;
   return iv_friendly_name_status;
 }
}

// static aka class member function (1.2.3.4 -> foo.com)
int IpAddress::resolve_to_hostname(const in_addr& quad_addr, char *hostname)
{
  struct hostent lookupResult;
  int loc_errno = 0;
  ACE_HOSTENT_DATA buffer;

  if (ACE_OS::gethostbyaddr_r((const char *)&quad_addr.s_addr, IPV4LEN, 
                          AF_INET, &lookupResult, buffer, &loc_errno)) {
    ACE_OS::strcpy( hostname, lookupResult.h_name); 
    return 0;
  }

  return loc_errno;
}

// static aka class member function (foo.com -> 1.2.3.4)
int IpAddress::resolve_to_address(const char *hostname, in_addr& quad_addr)
{
   struct hostent lookupResult;
   ACE_HOSTENT_DATA buffer;
   int loc_errno = 0;
   if (ACE_OS::gethostbyname_r ( hostname, &lookupResult, buffer, &loc_errno)) {
     if (lookupResult.h_length == sizeof(in_addr)) {
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
char * IpAddress::to_string()
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
addr_type IpAddress::get_type()	const
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
  valid_flag = FALSE;

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

  valid_flag=0;		// will get set TRUE if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_IPADDR: 
    {
      UdpAddress temp_udp(val.to_string());
      *this = temp_udp;	// valid_flag is set by the udp assignment
    }
    break;

    case sNMP_SYNTAX_OCTETS:
      if (((UdpAddress &)val).smival.value.string.len == UDPIPV4LEN){
         ACE_OS::memcpy(address_buffer,
		 ((UdpAddress &)val).smival.value.string.ptr,UDPIPV4LEN);
         iv_friendly_name[0] = 0;
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
  set_port(udpaddr.get_port());	// copy to port value
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
    valid_flag = FALSE;
    return FALSE;
  }
  // look for port info @ the end of the string
  // port can be delineated by a ':' or a '/'
  // if neither are present then just treat it 
  // like a normal IpAddress
  char *tmp;
  tmp = ACE_OS::strstr( buffer,":");
  if (tmp==NULL) 
    tmp = ACE_OS::strstr(buffer,"/");

  if ( tmp != NULL) {
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
    return 0;			// don't use uninitialized memory
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
char * UdpAddress::to_string()
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
 
    valid_flag=FALSE;
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
 
  valid_flag = FALSE;
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
 
char *NetbiosAddress::to_string()
{
  return (char *)output_buffer;
}
 
NetbiosAddress& NetbiosAddress::operator=( const NetbiosAddress &val)
{
 // protect against assignment from itself
  if ( this == &val )
      return *this;
 
  valid_flag = 0;         // will set to TRUE if really valid
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
 
char *NetbiosAddress::operator const char *() const
{
  return (char *)output_buffer;
}
 
SnmpSyntax& NetbiosAddress::operator=( SnmpSyntax &val)
{
  // protect against assignment from itself
  if ( this == &val )
      return *this;
 
  valid_flag = 0;         // will set to TRUE if really valid
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
    valid_flag=FALSE;
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

  valid_flag = FALSE;
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

  valid_flag=0;      	// will set to TRUE if really valid
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
// On success sets validity  TRUE or FALSE
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
  if (!inaddr || (ACE_OS::strlen( inaddr) >(sizeof(temp)-1))) return FALSE;
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
    return FALSE;

  // convert the string to all lower case
  // this allows hex values to be in upper or lower
  for (z=0;z< tmplen;z++)
    temp[z] = tolower(temp[z]);

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
    return FALSE;

  // separate the strings
  str1 = ( unsigned char *) temp;
  while( *str1 != separator) str1++;
  str2 = str1 + 1;
  *str1 = 0;
  str1= ( unsigned char *) temp;

  // check len of the network portion
  if ( ACE_OS::strlen((char *) str1) != 8) 
     return FALSE;

  // check len of mac portion
  if ( ACE_OS::strlen( (char *) str2) != 12) 
    return FALSE;

  // ok we like then lens, make sure that all chars are 0-f
  // check out the net id
  tmp = str1;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
	((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return FALSE;

  // check out the MAC address
  tmp = str2;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
	((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return FALSE;

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

  return TRUE;
}

//----[ IPX address char * cast ]--------------------------------------
IpxAddress::operator const char *() const
{
  return (char *)output_buffer;
}

//----[ IPX address get char representation ]--------------------------
char * IpxAddress::to_string()
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
          return TRUE;
       else
          return FALSE;
   }
   else
      return FALSE;
}


//------[ return the type ]----------------------------------
addr_type IpxAddress::get_type() const
{ 
  return type_ipx; 
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

  valid_flag = FALSE;
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

  valid_flag=0;      	// will set to TRUE if really valid
  if (val.valid()){
    switch (val.get_syntax()){
    case sNMP_SYNTAX_OCTETS: 
      {
        // See if it is of the Ipx address family
        // This handles IpxSockAddress == IpxAddress
        IpxSockAddress temp_ipx(val.to_string());
        if (temp_ipx.valid()){
          *this = temp_ipx;		// ipxsock = ipxsock
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

  (IpxAddress&)*this = ipxaddr; 	// use ancestor assignment for ipx addr
  set_socket(ipxaddr.get_socket());	// copy socket value
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
char * IpxSockAddress::to_string()
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
     valid_flag = FALSE;
     return FALSE;
   }
   // look for port info @ the end of the string
   // port can be delineated by a ':' or a '/'
   // if neither are present then just treat it 
   // like a normal IpAddress
   char *tmp;
   tmp = ACE_OS::strstr( buffer,"/");

   if (tmp != NULL)
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
 
    valid_flag=FALSE;
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

  valid_flag = FALSE;
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

  valid_flag=0;      	// will set to TRUE if really valid
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
// On success sets validity TRUE or FALSE
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
  if ( !inaddr || (ACE_OS::strlen( inaddr) > 30)) return FALSE;
  ACE_OS::strcpy( temp, inaddr);
  trim_white_space( temp);

  // bad total length check
  if ( ACE_OS::strlen(temp) != 17)  
     return FALSE;

  // check for colons
  if ((temp[2] != ':')||(temp[5] != ':')||(temp[8]!=':')||(temp[11]!=':')
         ||(temp[14] !=':'))
     return FALSE;

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
     temp[z] = tolower( temp[z]);

  
  // check out the MAC address
  tmp = ( unsigned char *) temp;
  while( *tmp != 0)
    if (((*tmp >= '0') && (*tmp <= '9'))||   // good 0-9
	((*tmp >= 'a') && (*tmp <= 'f')))    // or a-f
      tmp++;
    else
      return FALSE;

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

  return TRUE;
}

//----[ MAC address char * cast ]--------------------------------------
MacAddress::operator const char *() const
{
  return (char *)output_buffer;
}
//----[ MAC address get char representation ]--------------------------
char * MacAddress::to_string()
{
  return (char *)output_buffer;
}

//----[ MAC address format output ]---------------------------------
void MacAddress::format_output()
{
  if ( valid_flag)
    sprintf(output_buffer,"%02x:%02x:%02x:%02x:%02x:%02x",address_buffer[0],
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
        return ((((address_buffer[0] << 8) + address_buffer[1]) * HASH0)
                + (((address_buffer[2] << 8) + address_buffer[3]) * HASH1)
                + (((address_buffer[4] << 8) + address_buffer[5]) * HASH2));
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

//-----------[ constructor with a string argument ]----------------------
GenAddress::GenAddress( const char  *addr)
{
  if (ACE_OS::strlen(addr) == 0) {
    // initialize SMI info
    // BOK: this is generally not used for GenAddress,
    // but we need this to be a replica of the real address'
    // smival info so that operator=SnmpSyntax will work.
    smival.syntax = sNMP_SYNTAX_NULL;             // to be overridden
    smival.value.string.len = 0;                  // to be overridden
    smival.value.string.ptr = address_buffer;     // constant
 
    valid_flag = FALSE;
    address = 0;
    format_output();
    return;
  }

  // initialize SMI info
  // BOK: smival is generally not used for GenAddress, but 
  //      we need this to be a replica of the real address'
  //      smival info so that <class>::operator=SnmpSyntax 
  //      will work.
  smival.syntax = sNMP_SYNTAX_NULL;		// to be overridden
  smival.value.string.len = 0;			// to be overridden
  smival.value.string.ptr = address_buffer;	// constant

  address = 0;
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
GenAddress::GenAddress( const Address &addr)
{
  // initialize SMI info
  // BOK: this is generally not used for GenAddress,
  // but we need this to be a replica of the real address'
  // smival info so that operator=SnmpSyntax will work.
  smival.syntax = sNMP_SYNTAX_NULL;		// to be overridden
  smival.value.string.len = 0;			// to be overridden
  smival.value.string.ptr = address_buffer;	// constant

  valid_flag = FALSE;
  // make sure that the object is valid
  if (!addr.valid()) {
    address = 0;
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
{

  // initialize SMI info
  // BOK: this is generally not used for GenAddress,
  // but we need this to be a replica of the real address'
  // smival info so that operator=SnmpSyntax will work.
  smival.syntax = sNMP_SYNTAX_OCTETS;
  smival.value.string.len = 0;
  smival.value.string.ptr = address_buffer;

  valid_flag = FALSE;
  // make sure that the object is valid
  if (!addr.valid_flag) {
    address = 0;
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
};

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

  valid_flag = FALSE;
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

  valid_flag = FALSE;		// will get set to TRUE if really valid
  if ( address != 0) {
    delete address;
    address = 0;
  }

  if (val.valid())
  {
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
      // GenAddr = [UdpAdd|IpxAddr|IpxSock|MacAddr].  
      // Note, using the heuristic of octet str len to determine type of 
      // address to create is not accurate when address lengths are equal
      // (e.g., UDPIPV4LEN == MACLEN).  It gets worse if we add AppleTalk or 
      // OSI which use variable length addresses!
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
      if (address){
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
    if ( valid_flag && ((IpxSockAddress*)address)->get_socket())
    {
       format_output();
       return TRUE;   // ok its an ipxsock address
    }
	// otherwise delete it and try another
    delete address;

    // ipx address
    ACE_NEW_RETURN(address, IpxAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag)
    {
       format_output();
       return TRUE;   // ok its an ipx address
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
    if ( valid_flag && ((UdpAddress*)address)->get_port()) 
    {
       format_output();
       return TRUE;       // ok its a udp address
    }
    // otherwise delete it and try another
    delete address;

    // ip address
    ACE_NEW_RETURN(address, IpAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag) 
    {
       format_output();
       return TRUE;       // ok its an ip address
    }
	// otherwise delete it and try another
    delete address;

    // mac address
    ACE_NEW_RETURN(address, MacAddress( addr), -1);
    valid_flag = address->valid();
    if ( valid_flag)
    {
       format_output();
       return TRUE;    // ok, its a mac
    }
	// otherwise its invalid
    delete address;
    address = 0;
    format_output();
    return FALSE;
}

GenAddress::operator const char *() const
{
  if ( address != 0)
    return (const char *)*address;	// pass thru
  else
    return (char *)output_buffer;
}

// to_string form of the contained address
char * GenAddress::to_string() 
{
  if ( address != 0)
    return address->to_string();	// pass thru
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

