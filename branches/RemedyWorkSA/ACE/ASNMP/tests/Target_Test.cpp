
//=============================================================================
/**
 *  @file    Target_Test.cpp
 *
 *  $Id$
 *
 *  Test all the member functions of the Target class.
 *  Not sure if this object is really required or not in the new framework
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
#include "asnmp/octet.h"
#include "asnmp/target.h"
#include "test_config.h"

/*
  Percieved Problems with this CTarget aka UdpTarget Interface:

  1) can't set snmp version during constructor (default value?)
  2) doesn't use ANSI C++ String class (still uses char *)
  3) Makes it easy to mix up read and write comm strs (could be diff types)
  3) so many get/set's, leads one to rethink the design/use of UdpTarget
  4) Use of resolve_to_C smells like a HACK...
  5) No valid() member function returns 1 even if no address given..
  6) No to_string()?!  (Fixed)
  7) can't access retry, timeout parameters...
  8) can't assign or equate two UdpTargets

    UdpTarget( void);
    UdpTarget( const Address &address);
    UdpTarget( const UdpTarget &target);
    UdpTarget( const Address &address,              // address
             const char *read_community_name,     // read community name
             const char *write_community_name);   // write community name
    UdpTarget( const Address &address,                 // address
             const OctetStr &read_community_name,    // read community
             const OctetStr &write_community_name);  // write community
    ~UdpTarget();

    SnmpTarget *clone() const;
    void get_readcommunity( OctetStr& read_community_oct);
    void set_readcommunity( const OctetStr& read_community);
    void get_writecommunity( OctetStr &write_community_oct);
    void set_writecommunity( const OctetStr& write_community);
    void get_address( UdpAddress & address);
    int set_address( Address &address);
    snmp_version get_version();
    void set_version( const snmp_version v);

    UdpTarget& operator=( const UdpTarget& target);
    friend bool operator==( const UdpTarget &lhs, const UdpTarget &rhs);

 */

static void TestSnmpTarget()
{
  OctetStr rd("rd_comm"), wr("wr_comm");
  ACE_ASSERT(rd.valid() == 1);
  ACE_ASSERT(wr.valid() == 1);

  // constructor and get tests
  UdpAddress ga;
  ACE_ASSERT(ga.valid() == 0);

  UdpTarget c1;
  ACE_ASSERT(c1.valid() == 0);
  OctetStr a, b("public"), c("private");
  c1.get_read_community(a);
  ACE_ASSERT(a == b);
  c1.get_write_community(a);
  ACE_ASSERT(a == c);
  c1.get_address (ga);
  ACE_ASSERT(c1.get_version() == version1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpTarget:c1(\"\") [%s]\n",
    c1.to_string()));

  IpAddress ip("127.0.0.1");
  UdpTarget c2(ip);
  ACE_ASSERT(c2.valid() == 1);
  c2.get_address (ga);
  ACE_ASSERT(ga.valid() == 1);
  ACE_ASSERT(c2.get_version() == version1);
  ACE_ASSERT(ga.valid() == 1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpTarget:c2(\"\") [%s]\n",
    c2.to_string()));

  UdpTarget *c5 = new UdpTarget(c2);
  ACE_ASSERT(c5 != 0);
  ACE_ASSERT(c5->valid() == 1);
  c5->get_address (ga);
  ACE_ASSERT(ga.valid() == 1);
  ACE_ASSERT(c5->get_version() == version1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) UdpTarget:c5(\"\") [%s]\n",
    c5->to_string()));
  delete c5;

// these are not supported yet
//   ACE_ASSERT(c5 == c5);
//   c5 = c2;
//  ACE_ASSERT(c5 == c2);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Target_Test"));
  TestSnmpTarget();

  ACE_END_TEST;
  return 0;
}
