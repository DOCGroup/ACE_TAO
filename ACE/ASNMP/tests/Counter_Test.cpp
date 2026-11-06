
//=============================================================================
/**
 *  @file    Counter_Test.cpp
 *
 *  Test all the member functions of the Counter class. An Object
 *  representing an ASN.1 Counter SMI COUNTER SYNTAX.
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

#include <memory>
#include "ace/OS_main.h"
#include "asnmp/counter.h"
#include "test_config.h"

/*
   Counter32();
   Counter32( const unsigned long i);
   Counter32( const Counter32 &c);
*  SmiUINT32 get_syntax();
*  SnmpSyntax *clone() const;
*  SnmpSyntax& operator=( SnmpSyntax &val);
   Counter32& operator=( const Counter32 &uli);
   Counter32& operator=( const unsigned long i);
   operator unsigned long();

 --  comments tyis type appears to be a wrapper class and not
     a true SNMP counter. Practical for nms side,yet may lead to
     some confusion if implementing an agent with this class.

  Per RFC 1155 sec 3.2.3.3
   This application-wide type represents a non-negative integer which
   monotonically increases until it reaches a maximum value, when it
  wraps around and starts increasing again from zero. This memo
  specifies a maximum value of 2^32-1 for counters
*/

static void  TestCounter()
{
#if !defined (ACE_WIN32)
   long const l = LONG_MAX, nl = LONG_MIN;  // limits.h
   unsigned long const ul = ULONG_MAX, def = 0;
   int const i = INT_MAX, ni = INT_MIN;
   unsigned int const ui = UINT_MAX;
   unsigned short const us = 10;
   short const si = static_cast<short> (65535);

   // constructors
   Counter32 c1;
   ACE_ASSERT(c1 == def);
   Counter32 c2(l);
   ACE_ASSERT(c2 == static_cast<unsigned long> (l));
   Counter32 c3(nl);
   ACE_ASSERT(c3 == static_cast<unsigned long> (nl));
   Counter32 c4(ul);
   ACE_ASSERT(c4 == ul);
   Counter32 c5(i);
   ACE_ASSERT(c5 == static_cast<unsigned long> (i));
   Counter32 c6(ni);
   ACE_ASSERT(c6 == static_cast<unsigned long> (ni));
   Counter32 c7(ui);
   ACE_ASSERT(c7 == ui);
   std::unique_ptr<Counter32> c8 = std::make_unique<Counter32> (c5);
   ACE_ASSERT(c8 != nullptr);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c1(\"\") [%lu]\n",
    static_cast<unsigned long> (c1)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c2(\"%ld\") [%lu]\n",
    l, static_cast<unsigned long> (c2)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c3(\"%ld\") [%lu]\n",
    nl, static_cast<unsigned long> (c3)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c4(\"%lu\") [%lu]\n",
    ul, static_cast<unsigned long> (c4)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c5(\"%d\") [%lu]\n",
    i, static_cast<unsigned long> (c5)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c6(\"%d\") [%lu]\n",
    ni, static_cast<unsigned long> (c6)));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c7(\"%u\") [%lu]\n",
    ui, static_cast<unsigned long> (c7)));

  // assignent
  c1 = c2;  // obj
  ACE_ASSERT(c1 == c2);
  c1 = c1; // self
  ACE_ASSERT(c1 == c1);
  c1 = def; // unsigned long
  ACE_ASSERT(c1 == def);
  c1 = us; // unsigned short
  ACE_ASSERT(c1 == static_cast<unsigned long> (us));
  c1 = si; // unsigned short
  ACE_ASSERT(c1 == static_cast<unsigned long> (si));
#endif /*ACE_WIN32*/
}


int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Counter_Test"));

  TestCounter();

  ACE_END_TEST;
  return 0;
}
