
//=============================================================================
/**
 *  @file    Address_Test.cpp
 *
 *  $Id$
 *
 *  Test all the member functions of the Address family:
 *  GenAddress, MacAddress, IpxAddress, IpAddress, UdpAddress
 *  all which derive from abstract base class Address.
 *
 *
 *  @author Michael R. MacFaden <mrm@cisco.com>
 */
//=============================================================================

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Copyright 1997 Cisco Systems, Inc.

Permission to use, copy, modify, and distribute this software for any
purpose and without fee is hereby granted, provided that this
copyright and permission notice appear on all copies of the software and
supporting documentation, the name of Cisco Systems, Inc. not be used
in advertising or publicity pertaining to distribution of the
program without specific prior permission, and notice be given
in supporting documentation that modification, copying and distribution is by
permission of Cisco Systems, Inc.

Cisco Systems, Inc. makes no representations about the suitability of this
software for any purpose.  THIS SOFTWARE IS PROVIDED ``AS IS''
AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT
LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGMENT AND
FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL CISCO SYSTEMS, INC. BE
LIABLE FOR ANY DAMAGES ARISING OUT OF THIS LICENSE OR YOUR USE OF THE
SOFTWARE INCLUDING WITHOUT LIMITATION, DIRECT, INDIRECT OR CONSEQUENTIAL
DAMAGES.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "asnmp/address.h"
#include "test_config.h"

// test the GenAddress interface
/*
 */
static void TestGenAddr()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress: Tests: var(expected) [actual]\n"));

  // the constructors and destructors
  GenAddress *ga1 = new GenAddress("1.2.3.4"); // use ipv4 address
  ACE_ASSERT(ga1->valid() == 1);
  GenAddress ga2(*ga1); // copy constructor;
  ACE_ASSERT(ga2.valid() == 1);
  GenAddress ga3("localhost");
  ACE_ASSERT(ga3.valid() == 1);
  GenAddress ga4; // default constructor
  ACE_ASSERT(ga4.valid() == 0);
  GenAddress ga5; // default constructor
  ACE_ASSERT(ga5.valid() == 0);
  GenAddress ga6("127.0.0.1:7"); // udp address
  ACE_ASSERT(ga6.valid() == 1);
  GenAddress ga7("01234567.89ABcDeF0123"); // ipx address
  ACE_ASSERT(ga7.valid() == 1);
  IpAddress ip("1.2.3.4");
  ACE_ASSERT(ip.valid() == 1);
  GenAddress ga8(ip); // Address conversion
  ACE_ASSERT(ga8.valid() == 1);

  const char *ptr = (const char *)ga8;

  ACE_ASSERT(ga1 != 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga1(\"1.2.3.4\") [%s]\n",
    ga1->to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga2(ga1) [%s]\n",
    ga2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga3(\"localhost\") [%s]\n",
    ga3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga4(\"\") [%s]\n",
    ga4.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga6(\"127.0.0.1:7\") [%s]\n",
    ga6.to_string()));

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga7(\"01234567.89ABcDeF0123\") [%s]\n",
    ga7.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga8(\"1.2.3.4\") [%s]\n",
    ga8.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga8(\"1.2.3.4\") [%s]\n",
    ptr));

  // Test Assignment x = y, y = x
  ga5 = ga3;                    // regular assignment
  ga3 = ga3;                   // self assignment
  ACE_ASSERT(ga5 == ga3);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga5=ga3(\"localhost\") [%s]\n",
    ga5.to_string()));

  addr_type a = ga2.get_type();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga2.get_type(\"0\") [%d]\n",
     a));
  // udp address string
  a = ga6.get_type();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga2.get_type(\"2\") [%d]\n",
     a));

  // udp address string
  a = ga7.get_type();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) GenAddress:ga2.get_type(\"1\") [%d]\n",
     a));

  delete ga1, ga1 = 0;
}

/* public methods
  IpAddress( const char *inaddr = "");
  IpAddress( const IpAddress  &ipaddr);
  IpAddress( const GenAddress &genaddr);
  ~IpAddress();
  SnmpSyntax& operator=( SnmpSyntax &val);
  IpAddress& operator=( const IpAddress &ipaddress);
  SnmpSyntax *clone() const;
  char *resolve_hostname(int &status);
  virtual char *to_string() ;
  virtual operator const char *() const;
  void mask( const IpAddress& ipaddr);
  virtual addr_type get_type() const;
  virtual SmiUINT32 get_syntax();
  int is_loopback() const;
  int is_multicast() const;
  int is_broadcast() const;
  int is_arpanet() const;
 */
static void TestIpAddress()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress: Tests\n"));

  // constructors
  GenAddress ga("255.255.255.255");
  IpAddress ia1;
  IpAddress ia2("224.2.3.4");
  ACE_ASSERT(ia2.is_multicast());
  ACE_ASSERT(!ia2.is_loopback());
  IpAddress ia3("localhost");
  ACE_ASSERT(ia3.is_loopback());
  ACE_ASSERT(!ia3.is_multicast());
  IpAddress ia4(ia3);
  ACE_ASSERT(ia4.is_loopback());
  ACE_ASSERT(!ia4.is_multicast());
  IpAddress ia5(ga);
  ACE_ASSERT(ia5.is_broadcast());
  IpAddress ia6 = IpAddress("10.0.0.2");
  ACE_ASSERT(ia6.is_private());
  ACE_ASSERT(!ia6.is_multicast());
  ACE_ASSERT(!ia6.is_loopback());

  IpAddress ia7("172.16.0.1");
  ACE_ASSERT(ia7.is_private());

  IpAddress ia8("192.168.0.1");
  ACE_ASSERT(ia8.is_private());

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia1(\"\") [%s]\n",
    ia1.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia2(\"1.2.3.4\") [%s]\n",
    ia2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia3(\"127.0.0.1\") [%s]\n",
    ia3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia4(\"ia3\") [%s]\n",
    ia4.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia5(\"255.255.255.255\") [%s]\n",
    ia5.to_string()));

  // other routines
  int status = 1;
  const char *ptr = ia5.resolve_hostname(status);
  ACE_ASSERT(status == 0);
  ACE_ASSERT(ptr != 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia5.resolve_hostname():(\"\") [%s]\n",
    ptr));

  // now lets try one we setup with a hostname
  ptr = ia3.resolve_hostname(status);
  ACE_ASSERT(status == 0);
  ACE_ASSERT(ptr != 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:ia3.resolve_hostname()(\"localhost\") [%s]\n",
    ptr));

   ptr = (const char *)ia5;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpAddress:(const char *)(\"255.255.255.255\") [%s]\n",
    ptr));

   ia2 = ia3;
   ACE_ASSERT(ia2 == ia3);

   ia4.mask(ia3);  // mask with equal value should return same
   ACE_ASSERT(ia2 == ia3);

   ACE_ASSERT(ia1.get_type() == type_ip);
   ACE_ASSERT(ia1.valid() == 0);
   ACE_ASSERT(ia2.get_type() == type_ip);
   ACE_ASSERT(ia2.valid() == 1);
   ACE_ASSERT(ia3.get_type() == type_ip);
   ACE_ASSERT(ia3.valid() == 1);
   ACE_ASSERT(ia4.get_type() == type_ip);
   ACE_ASSERT(ia4.valid() == 1);
   ACE_ASSERT(ia5.get_type() == type_ip);
   ACE_ASSERT(ia5.valid() == 1);
}


// --------------- Netbios ---------------
/*
   NetbiosAddress( const char *inaddr = "");
   NetbiosAddress( const NetbiosAddress& nbaddr);
   NetbiosAddress( const GenAddress& genaddr);
   ~NetbiosAddress();
   virtual char *to_string();
   NetbiosAddress& operator=( const NetbiosAddress &nbaddr);
   nb_service get_service_type() const;
   void set_service_type(nb_service nbservice);
   virtual operator const char *() const;
   virtual SmiUINT32 get_syntax();
   SnmpSyntax& operator=( SnmpSyntax &val);
   SnmpSyntax *clone() const;
 */
static void TestNetbiosAddress()
{
  NetbiosAddress n1;
  ACE_ASSERT(n1.valid() == 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) NetbiosAddress:n1(\"\") [%s]\n",
    n1.to_string()));

  NetbiosAddress n2(n1);
  ACE_ASSERT(n2.valid() == 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) NetbiosAddress:n2(n1) [%s]\n",
    n2.to_string()));

  NetbiosAddress n3("pcname");
  ACE_ASSERT(n3.valid() == 1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) NetbiosAddress:n3(\"pcname\") [%s]\n",
    n3.to_string()));

  NetbiosAddress n4("abcdefghigjklmn");
  n4.set_service_type(nb_workstation);
  ACE_ASSERT(n4.valid() == 1);
  ACE_ASSERT(n4.get_service_type() == nb_workstation);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) NetbiosAddress:n4(\"abcdefghigjklmn\") [%s]\n",
    n4.to_string()));

  NetbiosAddress n5("abcdefghigjklmno0xx");
  ACE_ASSERT(n5.valid() == 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) NetbiosAddress:n4(\"abcdefghigjklmno0xx\") [%s]\n",
    n5.to_string()));

  n1 = n4;
  ACE_ASSERT(n1 == n4);
  ACE_ASSERT(ACE_OS::strcmp((const char *)n1, (const char *)n4) == 0);

  n1.set_service_type(nb_server);
  ACE_ASSERT(nb_server == n1.get_service_type());
}

// --------------- IPX ---------------
/*
  IpxAddress( void);
  IpxAddress( const char  *inaddr);
  IpxAddress( const IpxAddress  &ipxaddr);
  IpxAddress( const GenAddress &genaddr);
  ~IpxAddress();
  virtual SmiUINT32 get_syntax();
  SnmpSyntax& operator=( SnmpSyntax &val);
  IpxAddress& operator=( const IpxAddress &ipxaddress);
  int get_hostid( MacAddress& mac);
  SnmpSyntax *clone() const;
  virtual operator const char *() const;
  virtual addr_type get_type() const;

Ipx Address semantics:  Total length must be 21
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

 */

static void TestIpxAddress()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress: Tests\n"));
  IpxAddress xa1;
  ACE_ASSERT(xa1.valid() == 0);
  ACE_ASSERT(xa1.get_type() == type_ipx);
  GenAddress gen("01234567.0123456789AB");
  ACE_ASSERT(gen.valid() == 1);
  IpxAddress xa2("01234567.0123456789AB");
  ACE_ASSERT(xa2.get_type() == type_ipx);
  ACE_ASSERT(xa2.valid() == 1);
  IpxAddress xa3("01234567:0123456789AB");
  ACE_ASSERT(xa3.get_type() == type_ipx);
  ACE_ASSERT(xa3.valid() == 1);
  IpxAddress xa4("01234567-0123456789AB");
  ACE_ASSERT(xa4.get_type() == type_ipx);
  ACE_ASSERT(xa4.valid() == 1);
  IpxAddress xa5("01234567.012345-6789AB");
  ACE_ASSERT(xa5.get_type() == type_ipx);
  ACE_ASSERT(xa5.valid() == 1);
  IpxAddress xa6("01234567:012345-6789AB");
  ACE_ASSERT(xa6.get_type() == type_ipx);
  ACE_ASSERT(xa6.valid() == 1);
  IpxAddress xa7("01234567-012345-6789AB");
  ACE_ASSERT(xa7.get_type() == type_ipx);
  ACE_ASSERT(xa7.valid() == 1);
  IpxAddress xa8("01234567.");
  ACE_ASSERT(xa8.get_type() == type_ipx);
  ACE_ASSERT(xa8.valid() == 0);
  IpxAddress xa9(gen);
  ACE_ASSERT(xa9.valid() == 1);
  IpxAddress *xa10 = new IpxAddress(xa9);
  ACE_ASSERT(xa10->get_type() == type_ipx);
  ACE_ASSERT(xa10->valid() == 1);
  delete xa10;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa1(\"\") [%s]\n",
    xa1.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa2(\"01234567.0123456789AB\") [%s]\n",
    xa2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa3(\"01234567:0123456789A\") [%s]\n",
    xa3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa4(\"01234567-0123456789AB\") [%s]\n",
    xa4.to_string()));

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa5(\"01234567.012345-6789AB\") [%s]\n",
    xa5.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa6(\"01234567:012345-6789AB\") [%s]\n",
    xa6.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa7(\"01234567-012345-6789AB\") [%s]\n",
    xa7.to_string()));

  // assignment
  xa1 = xa3;
  ACE_ASSERT(xa1 == xa3);
  MacAddress mac;
  ACE_ASSERT(xa4.get_hostid(mac) == 1);
  ACE_ASSERT(mac.valid() == 1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa4:get_hostid(\"01:23:45:67:89:ab\") [%s]\n", mac.to_string()));

  const char *ptr = (const char *)xa7;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) IpxAddress:xa7-ptr(\"01234567-012345-6789AB\") [%s]\n", ptr));

}

/*
  MacAddress( void);

  MacAddress( const char  *inaddr);
  MacAddress( const MacAddress  &macaddr);
  MacAddress( const GenAddress &genaddr);
  ~MacAddress();
* SmiUINT32 get_syntax();
* SnmpSyntax& operator=( SnmpSyntax &val);
  MacAddress& operator=( const MacAddress &macaddress);
* SnmpSyntax *clone() const;
  virtual char *to_string();
  virtual operator const char *() const;
  virtual addr_type get_type() const;
  unsigned int hashFunction() const;

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

*/

static void TestMacAddress()
{

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress: Tests\n"));

  MacAddress ma1;
  ACE_ASSERT(ma1.valid() == 0);
  ACE_ASSERT(ma1.get_type() == type_mac);
  MacAddress ma2("01:23:45:67:89:AB");
  ACE_ASSERT(ma2.valid() == 1);
  ACE_ASSERT(ma2.get_type() == type_mac);
  MacAddress ma3("0123456789ABCEFGHI"); // invalid string
  ACE_ASSERT(ma3.valid() == 0);
  ACE_ASSERT(ma3.get_type() == type_mac);
  GenAddress ga("01:23:45:67:89:AB"); // mac address
  MacAddress ma4(ma2);
  ACE_ASSERT(ma4.valid() == 1);
  ACE_ASSERT(ma4.get_type() == type_mac);
  MacAddress ma5(ga);
  ACE_ASSERT(ma5.valid() == 0);
  ACE_ASSERT(ma5.get_type() == type_mac);

  ma1 = ma2;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma1(\"01:23:45:67:89:AB\") [%s]\n",
    ma1.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma2(\"01:23:45:67:89:AB\") [%s]\n",
    ma2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma3(\"\") [%s]\n",
    ma3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma4(\"01:23:45:67:89:AB\") [%s]\n",
    ma4.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma5(\"\") [%s]\n",
    ma5.to_string()));
  const char * ptr = (const char *)ma5;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma5(\"\") [%s]\n",
    ptr));

  // hashFunction crashes if not usedwith valid fn
  int x = ma2.hashFunction();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MacAddress:ma2.hashFunction(\"483201\") [%d]\n",
    x));
}

/*
  UdpAddress( void);
  UdpAddress( const char *inaddr);
  UdpAddress( const UdpAddress &udpaddr);
  UdpAddress( const GenAddress &genaddr);
  UdpAddress( const IpAddress &ipaddr);
  ~UdpAddress();
  SmiUINT32 get_syntax();
  SnmpSyntax& operator=( SnmpSyntax &val);
  UdpAddress& operator=( const UdpAddress &udpaddr);
  SnmpSyntax *clone() const;
  virtual char *to_string() ;
  virtual operator const char *() const;
  void set_port( const unsigned short p);
  unsigned short get_port() const;
  virtual addr_type get_type() const;

   // look for port info @ the end of the string
   // port can be delineated by a ':' or a '/'
   // if neither are present then just treat it
   // like a normal IpAddress

 */

static void TestUdpAddress()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress: Tests\n"));

  UdpAddress ua1;
  ACE_ASSERT(ua1.valid() == 0);
  ACE_ASSERT(ua1.get_type() == type_udp);

  // semantics of not setting the port here are bizzare?
  UdpAddress ua2("127.0.0.1:161");
  ACE_ASSERT(ua2.valid() == 1);
  ACE_ASSERT(ua2.get_type() == type_udp);

  UdpAddress ua3(ua2);
  ACE_ASSERT(ua3.valid() == 1);
  ACE_ASSERT(ua3.get_type() == type_udp);

  GenAddress ga("localhost");
  UdpAddress ua4(ga);
  ACE_ASSERT(ua4.valid() == 1);
  ACE_ASSERT(ua4.get_type() == type_udp);

  IpAddress ia("localhost");
  UdpAddress ua5(ga);
  ACE_ASSERT(ua5.valid() == 1);
  ACE_ASSERT(ua5.get_type() == type_udp);

  UdpAddress *ua6 = new UdpAddress("localhost:161");
  ACE_ASSERT(ua6->valid() == 1);
  ACE_ASSERT(ua6->get_type() == type_udp);

  UdpAddress ua7 = UdpAddress("localhost/162");
  ACE_ASSERT(ua7.valid() == 1);
  ACE_ASSERT(ua7.get_type() == type_udp);


  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua1(\"\") [%s]\n",
    ua1.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua2(\"127.0.0.1:161\") [%s]\n",
    ua2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua3(ua2)(\"127.0.0.1:161\") [%s]\n",
    ua3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua4(GenAddress)(\"127.0.0.1\") [%s]\n",
    ua4.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua5(IpAddress)(\"127.0.0.1\") [%s]\n",
    ua5.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua6()(\"localhost:161\") [%s]\n",
    ua6->to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua7()(\"localhost/162\") [%s]\n",
    ua7.to_string()));

  delete ua6; // destructor

  // assignement tests.
  ua1 = ua2;
  ACE_ASSERT(ua1 == ua2);
  ACE_ASSERT(ua1.valid() == 1);
  ua1 = ua1;
  ACE_ASSERT(ua1 == ua1);
  ACE_ASSERT(ua1.valid() == 1);

  //  set/get port
  ua1.set_port(333);
  ACE_ASSERT(ua1.get_port() == 333);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpAddress:ua1.set_port()(\"333\") [%s]\n",
    (const char *)ua1));

}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Address_Test"));

  TestGenAddr();
  TestIpAddress();
  TestUdpAddress();
  TestMacAddress();
  TestNetbiosAddress();
  TestIpxAddress();

  ACE_END_TEST;
  return 0;
}
