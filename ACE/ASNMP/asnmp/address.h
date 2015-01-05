

#ifndef ADDRESS_
#define ADDRESS_
//=============================================================================
/**
 *  @file    address.h
 *
 *  $Id$
 *
 *  Address class definition. Encapsulates various network
 *  addresses into easy to use, safe and portable classes.
 *
 *
 *  @author Peter E Mellquist
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

//----[ includes ]-----------------------------------------------------
// ACE OS Adaption layer
#include "ace/os_include/netinet/os_in.h"
#include "ace/os_include/os_netdb.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "asnmp/smival.h"
#include "asnmp/octet.h"

//----[ enumerated types ] --------------------------------
enum address_lengths {
   IPV4LEN=4,
   UDPIPV4LEN=6,
   IPV6LEN=16,
   UDPIPV6LEN=18,
   IPXLEN=10,
   IPXSOCKLEN=12,
   NETBIOSLEN=16,
   APPLETKLEN=3,
   DECNETLEN=2,
   MACLEN=6,
   HASH0LEN=19,
   HASH1LEN=13,
   HASH2LEN=7,
   MAX_ADDR_SZ=20,
   MAX_DISPLAY_SZ=MAXHOSTNAMELEN+1
};

enum addr_type {
  type_ip,
  type_ipx,
  type_udp,
  type_ipxsock,
  type_mac,
  type_nb,
  type_atk,
  type_decnet,
  type_e164,
  type_sip, // IPv6
  type_invalid
};

// TODO: get rest of nb service types added here
enum nb_service {
  nb_workstation = 0x0, nb_server = 0x20
};

//---[ forward declarations ]-----------------------------------------
class GenAddress;
class UdpAddress;
class IpxSockAddress;
class NetbiosAddress;
class SIPAddress; // aka ipv6

// TODO:
// class AtmE164Address;


//--------------------------------------------------------------------
//----[ Address class ]-----------------------------------------------
//--------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the abstract base class
 * Address. An Address is a unique network endpoint.
 */
class  ASNMP_Export Address: public  SnmpSyntax
{

public:
   /// allow destruction of derived classes
   virtual ~Address();

   /// overloaded equivlence operator, are two addresses equal?
   friend ASNMP_Export bool operator==( const Address &lhs,const Address &rhs);

   /// overloaded not equivlence operator, are two addresses not equal?
   friend ASNMP_Export bool operator!=( const Address &lhs,const Address &rhs);

   /// overloaded > operator, is a1 > a2
   friend ASNMP_Export bool operator>( const Address &lhs,const Address &rhs);

   /// overloaded >= operator, is a1 >= a2
   friend ASNMP_Export bool operator>=( const Address &lhs,const Address &rhs);

   /// overloaded < operator, is a1 < a2
   friend ASNMP_Export bool operator<( const Address &lhs,const Address &rhs);

   /// overloaded <= operator, is a1 <= a2
   friend ASNMP_Export bool operator<=( const Address &lhs,const Address &rhs);

   /// equivlence operator overloaded, are an address and a string equal?
   friend ASNMP_Export bool operator==( const Address &lhs,const char *rhs);

   /// overloaded not equivlence operator, are an address and string not equal?
   friend ASNMP_Export bool operator!=( const Address &lhs,const char *rhs);

   /// overloaded < , is an address greater than a string?
   friend ASNMP_Export bool operator>( const Address &lhs,const char *rhs);

   /// overloaded >=, is an address greater than or equal to a string?
   friend ASNMP_Export bool operator>=( const Address &lhs,const char *rhs);

   /// overloaded < , is an address less than a string?
   friend ASNMP_Export bool operator<( const Address &lhs,const char *rhs);

   /// overloaded <=, is an address less than or equal to a string?
   friend ASNMP_Export bool operator<=( const Address &lhs,const char *rhs);

   /// overloaded const char * cast
   virtual operator const char *() const = 0;

  /// verify the is the address object constructed ok
  virtual int valid() const;

  /// return a suitable buffer to contain the address
  virtual void to_octet(OctetStr& octet) const = 0;

  /// (pure virtual) syntax type
  virtual SmiUINT32 get_syntax() = 0;

  /// for non const [], allows reading and writing
  unsigned char& operator[]( const int position);

  /// get a printable ASCII value
  virtual const char *to_string() = 0;

  /// create a new instance of this Value
  virtual SnmpSyntax *clone() const = 0;

  /// return the type of address
  virtual addr_type get_type() const = 0;

  /// overloaded assignment operator
  virtual SnmpSyntax& operator=( SnmpSyntax &val) = 0;

  /// return a hash key
  virtual unsigned int hashFunction() const { return 0;};


protected:
  /// state of constructed object (1/0)
  /// addr internal representation
  int valid_flag;
  unsigned char address_buffer[MAX_ADDR_SZ];

  /// parse the address string
  /// redefined for each specific address subclass
  virtual int parse_address( const char * inaddr) =0;

  /// format the output
  /// redefined for each specific address subclass
  virtual void format_output() =0;

  /// a reused trimm white space method
  void trim_white_space( char * ptr);
};


//-----------------------------------------------------------------------
//---------[ IPv4 Address Class ]----------------------------------------
//-----------------------------------------------------------------------
/**
 * @class IpAddress
 *
 * @brief Defines the member functions for the concrete class IpAddress
 * An IP Version 4 Address is 4 bytes long and consists of a
 * Network, Sub Network, and host component.
 */
class ASNMP_Export IpAddress : public Address
{
public:
  /// default construct an IP address with a string
  IpAddress( const char *inaddr = "");

  /// construct an IP address with another IP address
  IpAddress( const IpAddress  &ipaddr);

  /// construct an IP address with a GenAddress
  IpAddress( const GenAddress &genaddr);

  /// destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)
  ~IpAddress();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// assignment to another IpAddress object overloaded
  IpAddress& operator=( const IpAddress &ipaddress);

  // TODO: add ability to set addr given long

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// return the DNS Fully Qualified Domain Name (host.domain)
  /// on failure returns dotted_quad string
  const char *resolve_hostname(int& was_found);

  /// return string representation of object (dotted quad returned)
  virtual const char *to_string() ;

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// logically AND two IPaddresses and
  /// return the new one
  void mask( const IpAddress& ipaddr);

  /// return the type
  virtual addr_type get_type() const;

  /// syntax type
  virtual SmiUINT32 get_syntax();

  /// is this the loopback address? 127.0.0.1/loopback/1.0.0.127.in-addr.arpa
  int is_loopback() const;

  /// determine if this is a multicast address
  int is_multicast() const;

  /// determine if this a broadcast address
  int is_broadcast() const;

  /// per RFC 1597,  private addresses are:: 10, 172.16, and 192.168.0
  int is_private() const;

  /// convert address into octet string format in network byte order
  virtual void to_octet(OctetStr& octet) const;

protected:
  char output_buffer[MAX_DISPLAY_SZ];           // output buffer

  /// friendly name storage
  char iv_friendly_name_[MAX_DISPLAY_SZ];

  /// did resolver call work? some addrs won't resolve
  int  iv_friendly_name_status_;

  /// redefined parse address
  /// specific to IP addresses
  virtual int parse_address( const char *inaddr);

  /// redefined format output
  /// specific to IP addresses
  virtual void format_output();

  /// parse a dotted string
  int parse_dotted_ipstring( const char *inaddr);

  /// using the currently defined address, do a gethostbyname()
  /// and try to fill up the name
  int addr_to_friendly();

  /// thread safe routine to lookup ip address given hostname
  /// return <> 0 on error
  static int resolve_to_address(const char *hostname, in_addr& quad_addr);

  /// thread safe routine to lookup name given ip address
  /// return <> 0 on error
  static int resolve_to_hostname(const in_addr& quad_addr, char *hostname);

};

//------------------------------------------------------------------------
//--------------[ DNS Iterator Class ]------------------------------------
//------------------------------------------------------------------------

/**
 * @class Address_Iter
 *
 * @brief Defines routines to obtain information on a hostname/FQDN
 * such as multiple addresses
 */
class ASNMP_Export Address_Iter
{
public:
  Address_Iter(const char *hostname); // fully qualified domain name, hostname

  /// did hostname resolve via DNS?
  int valid() const;

  /// how many addresses associated with this hostname
  int how_many_addresses();

  /// return next address
  int next(IpAddress& addr);

private:
  Address_Iter(const Address_Iter&);
  int valid_;                  // ctor status
  int count_;                   // number of addresses
  char **entry_;                // ptr to current address
  struct hostent lookupResult_;
  ACE_HOSTENT_DATA buffer_;
  int query_dns(const char *hostname);
};

//------------------------------------------------------------------------
//---------[ UDP/IPv4 Address Class ]-------------------------------------
//------------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the concrete class UdpAddress
 * A Udp Address consists of an IP Version 4 Address (IpAddress)
 * and a 2 byte unsigned port number.  (see /etc/services file)
 * User Datagram Protocol (UDP) is a best effort transport
 */
class  ASNMP_Export UdpAddress : public IpAddress
{
public:
  /// default constructor with a dotted string in the form of addr:port
  UdpAddress( const char *inaddr = "");

  /// construct an Udp address with another Udp address
  UdpAddress( const UdpAddress &udpaddr);

  /// construct a Udp address with a GenAddress
  UdpAddress( const GenAddress &genaddr);

  /// construct a Udp address with an IpAddress
  /// default port # to zero
  UdpAddress( const IpAddress &ipaddr);

  /// destructor
  ~UdpAddress();

  /// syntax type
  SmiUINT32 get_syntax();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// assignment to another IpAddress object overloaded
  UdpAddress& operator=( const UdpAddress &udpaddr);

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// output in the form of address:port
  virtual const char *to_string() ;

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// set the port number
  void set_port( const unsigned short p);

  /// get the port number
  unsigned short get_port() const;

  /// return the type
  virtual addr_type get_type() const;

protected:
  /// output buffer
  char output_buffer[MAX_DISPLAY_SZ];

  /// redefined parse address
  /// specific to IP addresses
  virtual int parse_address( const char *inaddr);

  /// redefined format output
  /// specific to IP addresses
  virtual void format_output();
};


//-------------------------------------------------------------------------
//---------[ 802.3 MAC Address Class ]-------------------------------------
//-------------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the concrete class MacAddress.
 * A Media Access Control Address consists of 48 bits as defined
 * in IEEE 802.3 specifications.
 */
class  ASNMP_Export MacAddress : public Address
{
public:
  /// constructor with a string argument
  MacAddress( const char  *inaddr = "");

  /// constructor with another MAC object
  MacAddress( const MacAddress  &macaddr);

  /// construct a MacAddress with a GenAddress
  MacAddress( const GenAddress &genaddr);

  /// destructor
  ~MacAddress();

  /// syntax type
  SmiUINT32 get_syntax();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// assignment to another IpAddress object overloaded
  MacAddress& operator=( const MacAddress &macaddress);

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// create a string to internal class storage representing object
  virtual const char *to_string();

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// return the type
  virtual addr_type get_type() const;

  /// return a hash key
  unsigned int hashFunction() const;

  /// return byte array of the mac address
  virtual void to_octet(OctetStr& octet) const;

protected:
  /// output buffer containing string representation of object
  char output_buffer[MAX_DISPLAY_SZ];

  /// redefined parse address for macs
  virtual int parse_address( const char *inaddr);

  /// redefined format output for MACs
  virtual void format_output();
};

//------------------------------------------------------------------------
//---------[ Netbios Address Class ]--------------------------------------
//------------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the concrete class NetbiosAddress.
 * The IBM/Microsoft address for NETBIOS, NETBEUI protocol transport.
 */
class  ASNMP_Export NetbiosAddress : public Address
{
public:

   /// default constructor with string arg
   NetbiosAddress( const char *inaddr = "");

   /// set name and service type
   NetbiosAddress( const char *inaddr, nb_service svc);

   /// copy constructor
   NetbiosAddress( const NetbiosAddress& nbaddr);

   /// construct with a GenAddress
   NetbiosAddress( const GenAddress& genaddr);

   ~NetbiosAddress();

   virtual const char *to_string();

   NetbiosAddress& operator=( const NetbiosAddress &nbaddr);

   /// retrieve the network service type
   nb_service get_service_type() const;

   /// set the service type (workstation, server, etc)
   void set_service_type(nb_service nbservice);

   /// const char * operator overloaded for streaming output
   virtual operator const char *() const;

   /// syntax type
   virtual SmiUINT32 get_syntax();

   /// copy an instance of this Value
   SnmpSyntax& operator=( SnmpSyntax &val);

   /// create a new instance of this Value
   SnmpSyntax *clone() const;

   /// output byte buffer containing netbios name
   virtual void to_octet(OctetStr& octet) const;

protected:
  void InitNBAddr(const char *inaddr);
  char output_buffer[MAX_DISPLAY_SZ];

  /// output buffer to hold string representation
  virtual void format_output();
  virtual int parse_address( const char  *inaddr);
  virtual addr_type get_type() const;
};

//------------------------------------------------------------------------
//---------[ DecNet Address Class ]---------------------------------------
//------------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the concrete class DecNetAddress.
 * DecNet Phase ? address consists of two octets (CISCO-TC.my)
 */
class  ASNMP_Export DecNetAddress : public Address
{
  public:
   /// default constructor with string arg
   DecNetAddress( const char *inaddr = "");

   /// copy constructor
   DecNetAddress( const DecNetAddress& decaddr);

   /// construct with a GenAddress
   DecNetAddress( const GenAddress& genaddr);

   ~DecNetAddress();

   virtual const char *to_string();

   DecNetAddress& operator=( const DecNetAddress &decaddr);

   /// convert address into octet string format 2 bytes of decnet address
   virtual void to_octet(OctetStr& octet) const;

   /// const char * operator overloaded for streaming output
   virtual operator const char *() const;

   /// syntax type
   virtual SmiUINT32 get_syntax();

   /// copy an instance of this Value
   SnmpSyntax& operator=( SnmpSyntax &val);

   /// create a new instance of this Value
   SnmpSyntax *clone() const;

   protected:
   virtual int parse_address( const char *inaddr);
   virtual addr_type get_type() const;
   virtual void format_output();
   char output_buffer[MAX_DISPLAY_SZ];
};

//------------------------------------------------------------------------
//---------[ AppleTalk Address Class ]------------------------------------
//------------------------------------------------------------------------
/**
 * @class
 *
 * @brief Defines the member functions for the concrete class DecNetAddress.
 * DecNet Phase ? address consists of two octets (CISCO-TC.my)
 */
class  ASNMP_Export AppleTalkAddress :  public Address
{
  public:
   /// default constructor with string arg
  AppleTalkAddress( const char *inaddr = "");

   /// copy constructor
   AppleTalkAddress( const AppleTalkAddress& atkaddr);

   /// construct with a GenAddress
   AppleTalkAddress( const GenAddress& genaddr);

   ~AppleTalkAddress();

   virtual const char *to_string();

   AppleTalkAddress& operator=( const AppleTalkAddress &atkaddr);

   /// convert address into octet string format 3 bytes of atk address
   virtual void to_octet(OctetStr& octet) const;

   /// get the host part of the address
   char get_host_address() const;

   /// set the host part of the address
   void set_host_address(const char);

   /// get the 2 byte atk network address
   short get_net_address() const;

   /// set the host 2 byte atk  network address
   void set_net_address(const short atknet);

   /// const char * operator overloaded for streaming output
   virtual operator const char *() const;

   /// syntax type
   virtual SmiUINT32 get_syntax();

   /// copy an instance of this Value
   SnmpSyntax& operator=( SnmpSyntax &val);

   /// create a new instance of this Value
   SnmpSyntax *clone() const;

   private:
   virtual int parse_address( const char *inaddr);
   virtual addr_type get_type() const;
   virtual void format_output();
   char output_buffer[MAX_DISPLAY_SZ];
};

//------------------------------------------------------------------------
//---------[ IPX Address Class ]------------------------------------------
//------------------------------------------------------------------------
/**
 * @class IpxAddress
 *
 * @brief Defines the member functions for the concrete class IpxAddress.
 * Novell's IPX (version ?) network protocol endpoint
 */
class ASNMP_Export IpxAddress : public Address
{
public:
  /// default constructor with a string arg
  IpxAddress( const char  *inaddr = "");

  /// constructor with another ipx object
  IpxAddress( const IpxAddress  &ipxaddr);

  /// construct with a GenAddress
  IpxAddress( const GenAddress &genaddr);

  /// destructor
  ~IpxAddress();

  /// syntax type
  virtual SmiUINT32 get_syntax();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// assignment to another IpxAddress object overloaded
  IpxAddress& operator=( const IpxAddress &ipxaddress);

  /// get the host id portion of an ipx address
  int get_hostid( MacAddress& mac);

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// create string represtation of object value
  virtual const char *to_string();

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// return the type
  virtual addr_type get_type() const;

  /// return byte sequence containing ipx address
  virtual void to_octet(OctetStr& octet) const;

protected:
  /// ipx format separator {:,/}
  char separator;

  /// output buffer to hold string representation
  char output_buffer[MAX_DISPLAY_SZ];

  /// redefined parse address for ipx strings
  virtual int parse_address( const char  *inaddr);

  /// redefined format output for ipx strings
  /// uses same separator as when constructed
  virtual void format_output();
};



//------------------------------------------------------------------------
//---------[ IpxSock Address Class ]--------------------------------------
//------------------------------------------------------------------------
/**
 * @class IpxSockAddress
 *
 * @brief Defines the member functions for the concrete class IpxAddress.
 * Novell's IPX (version ?) network protocol endpoint
 */
class ASNMP_Export IpxSockAddress : public IpxAddress
{
public:
  /// constructor with a dotted string
  IpxSockAddress( const char *inaddr = "");

  /// construct an Udp address with another Udp address
  IpxSockAddress( const IpxSockAddress &ipxaddr);

  ///constructor with a GenAddress
  IpxSockAddress( const GenAddress &genaddr);

  ///constructor with a IpxAddress
  /// default socket # is 0
  IpxSockAddress( const IpxAddress &ipxaddr);

  /// destructor
  ~IpxSockAddress();

  /// syntax type
  virtual SmiUINT32 get_syntax();

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// assignment to another IpxAddress object overloaded
  IpxSockAddress& operator=( const IpxSockAddress &ipxaddr);

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// set the socket number
  void set_socket( const unsigned short s);

  /// get the socket number
  unsigned short get_socket() const;

  /// create string representation of object value
  virtual const char *to_string();

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// return the type
  virtual addr_type get_type() const;

protected:
  /// output buffer to hold string representation of object
  char output_buffer[MAX_DISPLAY_SZ];

  /// redefined parse address for ipx strings
  virtual int parse_address( const char  *inaddr);

  /// redefined format output
  /// specific to IP addresses
  virtual void format_output();
};

//-------------------------------------------------------------------------
//--------[ Generic Address ]----------------------------------------------
//-------------------------------------------------------------------------
/**
 * @class GenAddress
 *
 * @brief Defines the member functions for the concrete class GenAddress.
 * This class attempts to determine an address type given a char string.
 */
class ASNMP_Export GenAddress : public Address
{
public:
  /// constructor with a string argument
  GenAddress( const char  *addr = "");

  /// constructor with an Address
  GenAddress( const Address &addr);

  /// constructor with another GenAddress
  GenAddress( const GenAddress &addr);

  /// destructor
  ~GenAddress();

  /// get the snmp syntax of the contained address
  SmiUINT32 get_syntax();

  /// create a new instance of this Value
  SnmpSyntax *clone() const;

  /// assignment of a GenAddress
  GenAddress& operator=( const GenAddress &addr);

  /// copy an instance of this Value
  SnmpSyntax& operator=( SnmpSyntax &val);

  /// string representation of object value
  virtual const char *to_string();

  /// const char * operator overloaded for streaming output
  virtual operator const char *() const;

  /// return the type
  virtual addr_type get_type() const;

  /// return the address as a octet sequence
  virtual void to_octet(OctetStr& octet) const;

protected:
  /// pointer to a a concrete address
  Address *address;

  /// output buffer of objects value
  char output_buffer[MAX_DISPLAY_SZ];

  /// redefined parse address for macs
  virtual int parse_address( const char *addr);

  /// format output for a generic address
  virtual void format_output();

  /// initialize smi data structure
  void init_smi();
};

#endif  //_ADDRESS
