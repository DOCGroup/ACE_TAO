// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    Oid_Test.cpp
//
// = DESCRIPTION
//  Test all the member functions of the Oid class. An Object
//  representing an ASN.1 Integer64 SMI OID SYNTAX.
//
// = AUTHOR
//    Michael R. MacFaden <mrm@cisco.com>
//
// ============================================================================
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
#include "asnmp/oid.h"
#include "test_config.h"

/*
  Oid( const char * dotted_oid_string = "", size_t size = -1);
  Oid ( const Oid &oid);
  Oid(const unsigned long *raw_oid, size_t oid_len);
  ~Oid();

  SmiUINT32 get_syntax();
  Oid& operator=( const Oid &oid);
  bool operator==( const Oid &lhs,const Oid &rhs);
  bool operator!=( const Oid &lhs,const Oid &rhs);
  bool operator<( const Oid &lhs,const Oid &rhs);
  bool operator<=( const Oid &lhs,const Oid &rhs);
  bool operator>( const Oid &lhs,const Oid &rhs);
  bool operator>=( const Oid &lhs,const Oid &rhs);
  Oid& operator+=( const char *a);
  Oid& operator+=( const unsigned long i);
  Oid& operator+=( const Oid &o);
  unsigned long & operator[]( size_t position);
  SmiLPOID oidval();
  void set_data( const char *dotted_oid_string);
  void set_data( const unsigned long *raw_oid, const size_t oid_len);
  size_t length() const;
  void trim( const size_t how_many = 1);
  int suboid( const size_t start, const size_t end, Oid& new_oid);
  int left_comparison( const unsigned long n, const Oid &o) const;
  int right_comparison( const unsigned long n, const Oid &o) const;
  int valid() const;
  char *to_string();
  SnmpSyntax *clone() const;
  SnmpSyntax& operator=( SnmpSyntax &val);
 */

static void OidTest()
{
  Oid d1;
  ACE_ASSERT(d1.valid() == 0);
  ACE_ASSERT(d1.length() == 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d1(\"\") [%s]\n",
    d1.to_string()));

  Oid d2("1.2.3");
  ACE_ASSERT(d2.valid() == 1);
  ACE_ASSERT(d2.length() == 3);
  ACE_ASSERT(d2[0] == (unsigned long) 1);
  ACE_ASSERT(d2[1] == (unsigned long) 2);
  ACE_ASSERT(d2[2] == (unsigned long) 3);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d2(\"1.2.3\") [%s]\n",
    d2.to_string()));

  Oid d3(d2);
  ACE_ASSERT(d3.valid() == 1);
  ACE_ASSERT(d3.length() == 3);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d3(\"d2\") [%s]\n",
    d3.to_string()));

  unsigned long t[3] = {2,3,4};
  Oid d4((unsigned long *)&t, sizeof(t)/sizeof(long));
  ACE_ASSERT(d4.valid() == 1);
  ACE_ASSERT(d4.length() == 3);
  ACE_ASSERT(d4[0] == (unsigned long) 2);
  ACE_ASSERT(d4[1] == (unsigned long) 3);
  ACE_ASSERT(d4[2] == (unsigned long) 4);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d4(\"long(2.3.4)\") [%s]\n",
    d4.to_string()));

  // suboid
  Oid d5;
  ACE_ASSERT(d4.suboid(d5, 1,1) == 0);  // 2,3,4, 1,1 == 3
  ACE_ASSERT(d5.length() == 1);
  ACE_ASSERT(d5.valid() == 1);
  ACE_ASSERT(d5[0] == 3);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d6::suboid(d5,1,1) [%s]\n",
    d5.to_string()));

  // trim
  Oid d7(d4);
  ACE_ASSERT(d7.valid() == 1);
  d7.trim();
  ACE_ASSERT(d7.length() == d4.length() -1);

  // compare methods
  ACE_ASSERT(d7.left_comparison( d7.length(), d7) == 0);
  ACE_ASSERT(d4.right_comparison( d4.length(), d4) == 0);

  // assignment
  d1 = d4;
  ACE_ASSERT(d1.valid() == 1);
  ACE_ASSERT(d1 == d4);
  d2 = "5.6.7";
  ACE_ASSERT(d2.valid() == 1);
  ACE_ASSERT(d2[2] == (unsigned long) 7);
  d1 = "8.9.10";
  ACE_ASSERT(d1.valid() == 1);

  // concat
  unsigned long ll = ULONG_MAX;
  d1 = "";
  d1 += (unsigned long)0;
  d1 += ll;
  d1 += ll;
  d1 += "0";
  ACE_ASSERT(d1.valid() == 1);
  ACE_ASSERT(d1.length() == 4);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:d1(0,max,max,0) [%s]\n",
    d1.to_string()));
  ACE_ASSERT(d1[0] == (unsigned long)0);
  ACE_ASSERT(d1[1] == ll);
  ACE_ASSERT(d1[2] == ll);
  ACE_ASSERT(d1[3] == (unsigned long)0);

  d2 += d1;
  ACE_ASSERT(d2.valid() == 1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Oid:(5.6.7.8.9.10) [%s]\n",
    d2.to_string()));

  // test out max Oid string...

  // relational operators oid,oid
  ACE_ASSERT(d2 == d2);
  ACE_ASSERT(!(d2 != d2));
  ACE_ASSERT(!(d2 < d2));
  ACE_ASSERT(!(d2 > d2));
  ACE_ASSERT(d2 >= d2);
  ACE_ASSERT(d2 <= d2);

}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Oid_Test"));
  OidTest();
  ACE_END_TEST;
  return 0;
}
