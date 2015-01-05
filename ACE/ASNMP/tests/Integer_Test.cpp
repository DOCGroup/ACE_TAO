// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Integer_Test.cpp
//
// = DESCRIPTION
//  Test all the member functions of the Integer class. An Object
//  representing an ASN.1 Integer64 SMI 32 bit Integer SYNTAX.
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
#include "asnmp/integer.h"
#include "test_config.h"

#if !defined (ACE_WIN32)
static long l = LONG_MAX, nl = LONG_MIN;  // limits.h
static unsigned long ul = ULONG_MAX, def = 0;
static int i = INT_MAX, ni = INT_MIN;
static unsigned int ui = UINT_MAX;
static unsigned short us = 10;
static unsigned short si = 32768;
 #endif /*ACE_WIN32*/

/*
     SnmpInt32( void);
     SnmpInt32 (const long i);
     SnmpInt32 (const SnmpInt32 &c);
     virtual ~SnmpInt32();
     virtual SmiUINT32 get_syntax();
     SnmpInt32& operator=( const long i);
     SnmpInt32& operator=( const SnmpInt32 &li);
     operator long();
     char *to_string();
     SnmpSyntax *clone() const;
     SnmpSyntax& operator=( SnmpSyntax &val);
     int valid() const;
 */

static void TestInteger32()
{
#if !defined (ACE_WIN32)
   // constructors
   SnmpInt32 i1;
   ACE_ASSERT(i1 == static_cast<long> (def));
   SnmpInt32 i2(l);
   ACE_ASSERT(i2 == l);
   SnmpInt32 i3(nl);
   ACE_ASSERT(i3 == nl);
   SnmpInt32 i4(ul);
   ACE_ASSERT(i4 == static_cast<long> (ul));
   SnmpInt32 i5(i);
   ACE_ASSERT(i5 == i);
   SnmpInt32 i6(ni);
   ACE_ASSERT(i6 == ni);
   SnmpInt32 i7(ui);
   ACE_ASSERT(i7 == static_cast<long> (ui));
   SnmpInt32 *i8 = new SnmpInt32(i5);
   ACE_ASSERT(i8 != 0);
   delete i8;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i1(\"\") [%u]\n",
    (unsigned long)i1));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i2(\"%u\") [%u]\n",
    l, (unsigned long)i2));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i3(\"%u\") [%u]\n",
    nl, (unsigned long)i3));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i4(\"%u\") [%u]\n",
    ul, (unsigned long)i4));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i5(\"%u\") [%u]\n",
    i, (unsigned long)i5));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i6(\"%u\") [%u]\n",
    ni, (unsigned long)i6));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) i7(\"%u\") [%u]\n",
    ui, (unsigned long)i7));

  // assignent
  i1 = i2;  // obj
  ACE_ASSERT(i1 == i2);
  i1 = i1; // self
  ACE_ASSERT(i1 == i1);
  i1 = def; // unsigned long
  ACE_ASSERT(i1 == static_cast<long> (def));
  i1 = us; // unsigned short
  ACE_ASSERT(i1 == static_cast<long> (us));
  i1 = si; // unsigned short
  ACE_ASSERT(i1 == static_cast<long> (si));
 #endif /*ACE_WIN32*/
}

/*
     SnmpUInt32( void);
     SnmpUInt32 (const unsigned long i);
     SnmpUInt32( const SnmpUInt32 &c);
     virtual ~SnmpUInt32();
     virtual SmiUINT32 get_syntax();
     SnmpUInt32& operator=( const unsigned long i);
     SnmpUInt32& operator=( const SnmpUInt32 &uli);
     operator unsigned long();
     virtual char *to_string();
     virtual SnmpSyntax *clone() const;
     SnmpSyntax& operator=( SnmpSyntax &val);
     int valid() const;
 */
static void TestUnsignedInteger32()
{
#if !defined (ACE_WIN32)
  // constructors
   SnmpUInt32 u1;
   ACE_ASSERT(u1 == def);
   SnmpUInt32 u2(l);
   ACE_ASSERT(u2 == static_cast<unsigned long> (l));
   SnmpUInt32 u3(nl);
   ACE_ASSERT(u3 == static_cast<unsigned long> (nl));
   SnmpUInt32 u4(ul);
   ACE_ASSERT(u4 == ul);
   SnmpUInt32 u5(i);
   ACE_ASSERT(u5 == static_cast<unsigned long> (i));
   SnmpUInt32 u6(ni);
   ACE_ASSERT(u6 == static_cast<unsigned long> (ni));
   SnmpUInt32 u7(ui);
   ACE_ASSERT(u7 == ui);
   SnmpUInt32 *u8 = new SnmpUInt32(u5);
   ACE_ASSERT(u8 != 0);
   delete u8;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u1(\"\") [%u]\n",
    (unsigned long)u1));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u2(\"%u\") [%u]\n",
    l, (unsigned long)u2));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u3(\"%u\") [%u]\n",
    nl, (unsigned long)u3));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u4(\"%u\") [%u]\n",
    ul, (unsigned long)u4));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u5(\"%u\") [%u]\n",
    i, (unsigned long)u5));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u6(\"%u\") [%u]\n",
    ni, (unsigned long)u6));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) u7(\"%u\") [%u]\n",
    ui, (unsigned long)u7));

 // assignent
  u1 = u2;  // obj
  ACE_ASSERT(u1 == u2);
  u1 = u1; // self
  ACE_ASSERT(u1 == u1);
  u1 = def; // unsigned long
  ACE_ASSERT(u1 == def);
  u1 = us; // unsigned short
  ACE_ASSERT(u1 == static_cast<unsigned long> (us));
  u1 = si; // unsigned short
  ACE_ASSERT(u1 == static_cast<unsigned long> (si));
#endif /*ACE_WIN32 */
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Integer_Test"));

  TestInteger32();

  TestUnsignedInteger32();

  ACE_END_TEST;
  return 0;
}
