/* -*-C++-*- */
#ifndef ADDRESS_
#define ADDRESS_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    address.h
//
// = DESCRIPTION
//  Address class definition. Encapsulates various network
//  addresses into easy to use, safe and portable classes.
//
// = AUTHOR
//    Peter E Mellquist 
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

//----[ includes ]-----------------------------------------------------
// ACE OS Adaption layer
#include "ace/OS.h"
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
   HASH0=19,
   HASH1=13, 
   HASH2=7,
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
class  ACE_Export Address: public  SnmpSyntax 
  // = TITLE
  //     Defines the member functions for the abstract base class 
  //     Address. An Address is a unique network endpoint.
{

public:
   virtual ~Address();
   // allow destruction of derived classes

   friend int operator==( const Address &lhs,const Address &rhs);
   // overloaded equivlence operator, are two addresses equal?

   friend int operator!=( const Address &lhs,const Address &rhs);
   // overloaded not equivlence operator, are two addresses not equal?

   friend int operator>( const Address &lhs,const Address &rhs);
   // overloaded > operator, is a1 > a2

   friend int operator>=( const Address &lhs,const Address &rhs);
   // overloaded >= operator, is a1 >= a2

   friend int operator<( const Address &lhs,const Address &rhs);
   // overloaded < operator, is a1 < a2

   friend int operator<=( const Address &lhs,const Address &rhs);
   // overloaded <= operator, is a1 <= a2

   friend int operator==( const Address &lhs,const char *rhs);
   // equivlence operator overloaded, are an address and a string equal?

   friend int operator!=( const Address &lhs,const char *rhs);
   // overloaded not equivlence operator, are an address and string not equal?

   friend int operator>( const Address &lhs,const char *rhs);
   // overloaded < , is an address greater than a string?

   friend int operator>=( const Address &lhs,const char *rhs);
   // overloaded >=, is an address greater than or equal to a string?

   friend int operator<( const Address &lhs,const char *rhs);
   // overloaded < , is an address less than a string?

   friend int operator<=( const Address &lhs,const char *rhs);
   // overloaded <=, is an address less than or equal to a string?

   virtual operator const char *() const = 0;
   // overloaded const char * cast 

  virtual int valid() const;            
  // verify the is the address object constructed ok

  virtual void to_octet(OctetStr& octet) const = 0; 
  // return a suitable buffer to contain the address 

  virtual SmiUINT32 get_syntax() = 0;
  // (pure virtual) syntax type

  unsigned char& operator[]( const int position);
  // for non const [], allows reading and writing

  virtual char *to_string() = 0;
  // get a printable ASCII value

  virtual SnmpSyntax *clone() const = 0;
  // create a new instance of this Value

  virtual addr_type get_type() const = 0; 
  // return the type of address

  virtual SnmpSyntax& operator=( SnmpSyntax &val) = 0;
  // overloaded assignment operator

  virtual unsigned int hashFunction() const { return 0;};
  // return a hash key


protected:
  int valid_flag;
  // state of constructed object (TRUE/FALSE)
  unsigned char address_buffer[MAX_ADDR_SZ]; 
  // addr internal representation

  virtual int parse_address( const char * inaddr) =0;
  // parse the address string
  // redefined for each specific address subclass

  virtual void format_output() =0;
  // format the output
  // redefined for each specific address subclass

  void trim_white_space( char * ptr);
  // a reused trimm white space method
};


//-----------------------------------------------------------------------
//---------[ IPv4 Address Class ]----------------------------------------
//-----------------------------------------------------------------------
class ACE_Export IpAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class IpAddress
  //     An IP Version 4 Address is 4 bytes long and consists of a
  //     Network, Sub Network, and host component. 
{
public:
  IpAddress( const char *inaddr = "");
  // default construct an IP address with a string

  IpAddress( const IpAddress  &ipaddr);
  // construct an IP address with another IP address

  IpAddress( const GenAddress &genaddr);
  // construct an IP address with a GenAddress

  ~IpAddress();
  // destructor (ensure that SnmpSyntax::~SnmpSyntax() is overridden)

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

  IpAddress& operator=( const IpAddress &ipaddress);
  // assignment to another IpAddress object overloaded

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  char *friendly_name(int &status);
  // return the friendly name
  // returns a NULL string if there isn't one

  virtual char *to_string() ;
  // return string representation of object

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  void mask( const IpAddress& ipaddr);
  // logically AND two IPaddresses and
  // return the new one

  virtual addr_type get_type() const;
  // return the type

  virtual SmiUINT32 get_syntax();
  // syntax type

  int is_loopback() const;
  // is this the loopback address? 127.0.0.1/loopback/1.0.0.127.in-addr.arpa

  int is_multicast() const;
  // determine if this is a multicast address 

  int is_broadcast() const;
  // determine if this a broadcast address 

  int is_private() const;
  // per RFC 1597,  private addresses are:: 10, 172.16, and 192.168.0 

  virtual void to_octet(OctetStr& octet) const;
  // convert address into octet string format in network byte order

protected:
  char output_buffer[MAX_DISPLAY_SZ];           // output buffer

  char iv_friendly_name[MAX_DISPLAY_SZ];
  // friendly name storage

  int  iv_friendly_name_status;
  // did resolver call work? some addrs won't resolve 

  virtual int parse_address( const char *inaddr);
  // redefined parse address
  // specific to IP addresses
   
  virtual void format_output();
  // redefined format output
  // specific to IP addresses

  int parse_dotted_ipstring( const char *inaddr);
  // parse a dotted string

  int addr_to_friendly();
  // using the currently defined address, do a gethostbyname()
  // and try to fill up the name

  static int resolve_to_address(const char *hostname, in_addr& quad_addr);
  // thread safe routine to lookup ip address given hostname
  // return <> 0 on error

  static int resolve_to_hostname(const in_addr& quad_addr, char *hostname);
  // thread safe routine to lookup name given ip address
  // return <> 0 on error
};

//------------------------------------------------------------------------
//---------[ UDP/IPv4 Address Class ]-------------------------------------
//------------------------------------------------------------------------
class  ACE_Export UdpAddress : public IpAddress 
  // = TITLE
  //     Defines the member functions for the concrete class UdpAddress
  //     A Udp Address consists of an IP Version 4 Address (IpAddress)
  //     and a 2 byte unsigned port number.  (see /etc/services file)
  //     User Datagram Protocol (UDP) is a best effort transport
{
public:
  UdpAddress( const char *inaddr = "");
  // default constructor with a dotted string in the form of addr:port

  UdpAddress( const UdpAddress &udpaddr);
  // construct an Udp address with another Udp address

  UdpAddress( const GenAddress &genaddr);
  // construct a Udp address with a GenAddress

  UdpAddress( const IpAddress &ipaddr);
  // construct a Udp address with an IpAddress
  // default port # to zero

  ~UdpAddress();
  // destructor

  SmiUINT32 get_syntax();
  // syntax type

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

  UdpAddress& operator=( const UdpAddress &udpaddr);
  // assignment to another IpAddress object overloaded

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  virtual char *to_string() ;
  // output in the form of address:port

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  void set_port( const unsigned short p);
  // set the port number

  unsigned short get_port() const; 
  // get the port number

  virtual addr_type get_type() const;
  // return the type

protected:
  char output_buffer[MAX_DISPLAY_SZ];           
  // output buffer

  virtual int parse_address( const char *inaddr);
  // redefined parse address
  // specific to IP addresses

  virtual void format_output();
  // redefined format output
  // specific to IP addresses
};


//-------------------------------------------------------------------------
//---------[ 802.3 MAC Address Class ]-------------------------------------
//-------------------------------------------------------------------------
class  ACE_Export MacAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class MacAddress.
  //     A Media Access Control Address consists of 48 bits as defined 
  //     in IEEE 802.3 specifications.
{
public:
  MacAddress( const char  *inaddr = "");
  // constructor with a string argument

  MacAddress( const MacAddress  &macaddr);
  // constructor with another MAC object

  MacAddress( const GenAddress &genaddr);
  // construct a MacAddress with a GenAddress

  ~MacAddress();
  // destructor 

  SmiUINT32 get_syntax();
  // syntax type

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

  MacAddress& operator=( const MacAddress &macaddress);
  // assignment to another IpAddress object overloaded

  SnmpSyntax *clone() const; 
  // create a new instance of this Value

  virtual char *to_string();
  // create a string to internal class storage representing object

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  virtual addr_type get_type() const;
  // return the type

  unsigned int hashFunction() const;
  // return a hash key

  virtual void to_octet(OctetStr& octet) const;
  // return byte array of the mac address 

protected:
  char output_buffer[MAX_DISPLAY_SZ];           
  // output buffer containing string representation of object

  virtual int parse_address( const char *inaddr);
  // redefined parse address for macs

  virtual void format_output();
  // redefined format output for MACs
};

//------------------------------------------------------------------------
//---------[ Netbios Address Class ]--------------------------------------
//------------------------------------------------------------------------
class  ACE_Export NetbiosAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class NetbiosAddress.
  //     The IBM/Microsoft address for NETBIOS, NETBEUI protocol transport. 
{
public:

   NetbiosAddress( const char *inaddr = "");
   // default constructor with string arg

   NetbiosAddress( const char *inaddr, nb_service svc);
   // set name and service type 

   NetbiosAddress( const NetbiosAddress& nbaddr); 
   // copy constructor

   NetbiosAddress( const GenAddress& genaddr); 
   // construct with a GenAddress
   
   ~NetbiosAddress(); 

   virtual char *to_string();

   NetbiosAddress& operator=( const NetbiosAddress &nbaddr);

   nb_service get_service_type() const;
   // retrieve the network service type

   void set_service_type(nb_service nbservice);
   // set the service type (workstation, server, etc)

   virtual operator const char *() const;
   // const char * operator overloaded for streaming output

   virtual SmiUINT32 get_syntax();
   // syntax type

   SnmpSyntax& operator=( SnmpSyntax &val); 
   // copy an instance of this Value

   SnmpSyntax *clone() const; 
   // create a new instance of this Value

   virtual void to_octet(OctetStr& octet) const;
   // output byte buffer containing netbios name 

protected:
  void InitNBAddr(const char *inaddr);
  char output_buffer[MAX_DISPLAY_SZ];           

  virtual void format_output(); 
  // output buffer to hold string representation
  virtual int parse_address( const char  *inaddr);
  virtual addr_type get_type() const;
};

//------------------------------------------------------------------------
//---------[ DecNet Address Class ]---------------------------------------
//------------------------------------------------------------------------
class  ACE_Export DecNetAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class DecNetAddress.
  //     DecNet Phase ? address consists of two octets (CISCO-TC.my)
{
  public:
   DecNetAddress( const char *inaddr = "");
   // default constructor with string arg
 
   DecNetAddress( const DecNetAddress& decaddr);
   // copy constructor
 
   DecNetAddress( const GenAddress& genaddr);
   // construct with a GenAddress
 
   ~DecNetAddress();
 
   virtual char *to_string();
 
   DecNetAddress& operator=( const DecNetAddress &decaddr);

   virtual void to_octet(OctetStr& octet) const;
   // convert address into octet string format 2 bytes of decnet address 
 
   virtual operator const char *() const;
   // const char * operator overloaded for streaming output
 
   virtual SmiUINT32 get_syntax();
   // syntax type
 
   SnmpSyntax& operator=( SnmpSyntax &val);
   // copy an instance of this Value
 
   SnmpSyntax *clone() const;
   // create a new instance of this Value

   protected:  
   virtual int parse_address( const char *inaddr);
   virtual addr_type get_type() const;
   virtual void format_output();
   char output_buffer[MAX_DISPLAY_SZ];           
};

//------------------------------------------------------------------------
//---------[ AppleTalk Address Class ]------------------------------------
//------------------------------------------------------------------------
class  ACE_Export AppleTalkAddress :  public Address 
  // = TITLE
  //     Defines the member functions for the concrete class DecNetAddress.
  //     DecNet Phase ? address consists of two octets (CISCO-TC.my)
{
  public:
  AppleTalkAddress( const char *inaddr = "");
   // default constructor with string arg
 
   AppleTalkAddress( const AppleTalkAddress& atkaddr);
   // copy constructor
 
   AppleTalkAddress( const GenAddress& genaddr);
   // construct with a GenAddress
 
   ~AppleTalkAddress();
 
   virtual char *to_string();
 
   AppleTalkAddress& operator=( const AppleTalkAddress &atkaddr);
 
   virtual void to_octet(OctetStr& octet) const;
   // convert address into octet string format 3 bytes of atk address 

   char get_host_address() const;
   // get the host part of the address

   void set_host_address(const char);
   // set the host part of the address

   short get_net_address() const;
   // get the 2 byte atk network address

   void set_net_address(const short atknet);
   // set the host 2 byte atk  network address
   
   virtual operator const char *() const;
   // const char * operator overloaded for streaming output
 
   virtual SmiUINT32 get_syntax();
   // syntax type
 
   SnmpSyntax& operator=( SnmpSyntax &val);
   // copy an instance of this Value
 
   SnmpSyntax *clone() const;
   // create a new instance of this Value
 
   private:
   virtual int parse_address( const char *inaddr);
   virtual addr_type get_type() const;
   virtual void format_output();
   char output_buffer[MAX_DISPLAY_SZ];           
};

//------------------------------------------------------------------------
//---------[ IPX Address Class ]------------------------------------------
//------------------------------------------------------------------------
class ACE_Export IpxAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class IpxAddress.
  //     Novell's IPX (version ?) network protocol endpoint
{
public:
  IpxAddress( const char  *inaddr = "");
  // default constructor with a string arg

  IpxAddress( const IpxAddress  &ipxaddr);
  // constructor with another ipx object

  IpxAddress( const GenAddress &genaddr);
  // construct with a GenAddress

  ~IpxAddress();
  // destructor 

  virtual SmiUINT32 get_syntax();
  // syntax type

  SnmpSyntax& operator=( SnmpSyntax &val); 
  // copy an instance of this Value

  IpxAddress& operator=( const IpxAddress &ipxaddress);
  // assignment to another IpAddress object overloaded

  int get_hostid( MacAddress& mac);
  // get the host id portion of an ipx address

  SnmpSyntax *clone() const; 
  // create a new instance of this Value

  virtual char *to_string();
  // create string represtation of object value

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  virtual addr_type get_type() const;
  // return the type

  virtual void to_octet(OctetStr& octet) const;
  // return byte sequence containing ipx address 

protected:
  char separator;
  // ipx format separator {:,/}

  char output_buffer[MAX_DISPLAY_SZ];           
  // output buffer to hold string representation

  virtual int parse_address( const char  *inaddr);
  // redefined parse address for ipx strings

  virtual void format_output();
  // redefined format output for ipx strings
  // uses same separator as when constructed
};



//------------------------------------------------------------------------
//---------[ IpxSock Address Class ]--------------------------------------
//------------------------------------------------------------------------
class ACE_Export IpxSockAddress : public IpxAddress 
  // = TITLE
  //     Defines the member functions for the concrete class IpxAddress.
  //     Novell's IPX (version ?) network protocol endpoint 
{
public:
  IpxSockAddress( const char *inaddr = "");
  // constructor with a dotted string

  IpxSockAddress( const IpxSockAddress &ipxaddr);
  // construct an Udp address with another Udp address

  IpxSockAddress( const GenAddress &genaddr);
  //constructor with a GenAddress

  IpxSockAddress( const IpxAddress &ipxaddr);
  //constructor with a IpxAddress
  // default socket # is 0

  ~IpxSockAddress();
  // destructor

  virtual SmiUINT32 get_syntax();
  // syntax type

  SnmpSyntax& operator=( SnmpSyntax &val); 
  // copy an instance of this Value

  IpxSockAddress& operator=( const IpxSockAddress &ipxaddr);
  // assignment to another IpAddress object overloaded

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  void set_socket( const unsigned short s);
  // set the socket number

  unsigned short get_socket() const;
  // get the socket number

  virtual char *to_string();
  // create string representation of object value

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  virtual addr_type get_type() const;
  // return the type

protected:
  char output_buffer[MAX_DISPLAY_SZ];           
  // output buffer to hold string representation of object

  virtual int parse_address( const char  *inaddr);
  // redefined parse address for ipx strings

  virtual void format_output();
  // redefined format output
  // specific to IP addresses
};

//-------------------------------------------------------------------------
//--------[ Generic Address ]----------------------------------------------
//-------------------------------------------------------------------------
class ACE_Export GenAddress : public Address 
  // = TITLE
  //     Defines the member functions for the concrete class GenAddress.
  //     This class attempts to determine an address type given a char string.
{
public:
  GenAddress( const char  *addr = "");
  // constructor with a string argument

  GenAddress( const Address &addr);
  // constructor with an Address

  GenAddress( const GenAddress &addr);
  // constructor with another GenAddress

  ~GenAddress();
  // destructor

  SmiUINT32 get_syntax();
  // get the snmp syntax of the contained address

  SnmpSyntax *clone() const;
  // create a new instance of this Value

  GenAddress& operator=( const GenAddress &addr);
  // assignment of a GenAddress

  SnmpSyntax& operator=( SnmpSyntax &val);
  // copy an instance of this Value

  virtual char *to_string();
  // string representation of object value

  virtual operator const char *() const;
  // const char * operator overloaded for streaming output

  virtual addr_type get_type() const;
  // return the type

  virtual void to_octet(OctetStr& octet) const;
  // return the address as a octet sequence
 
protected:
  Address *address;
  // pointer to a a concrete address

  char output_buffer[MAX_DISPLAY_SZ];           
  // output buffer of objects value

  virtual int parse_address( const char *addr);
  // redefined parse address for macs

  virtual void format_output();
  // format output for a generic address

  void init_smi();
  // initialize smi data structure
};

#endif  //_ADDRESS
