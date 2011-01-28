// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Guage_Test.cpp
//
// = DESCRIPTION
//  Test all the member functions of the Guage class. An Object
//  representing an ASN.1 Counter SMI GUAGE SYNTAX.
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
#include "asnmp/gauge.h"
#include "test_config.h"

/*
   Gauge32( void);
   Gauge32( const unsigned long i);
   Gauge32 ( const Gauge32 &g);
   ~Gauge32();
   SmiUINT32 get_syntax();
   SnmpSyntax *clone() const;
   Gauge32& operator=( const Gauge32 &uli);
   Gauge32& operator=( const unsigned long i);
   operator unsigned long();
   SnmpSyntax& operator=( SnmpSyntax &val);

--  What is a Gauge? According to RFC 1155 section: 3.2.3.4
   This  application-wide type represents a non-negative integer
   which may increase or decrease, but which latches at a maximum
   value of 2^32-1 (4294967295 dec) for gauges.
 */
static void
TestGuage (void)
{
#if !defined (ACE_WIN32)
   long l = LONG_MAX, nl = LONG_MIN;  // limits.h
   unsigned long ul = ULONG_MAX, def = 0;
   int i = INT_MAX, ni = INT_MIN;
   unsigned int ui = UINT_MAX;
   unsigned short us = 10;
   short si = static_cast<short> (65535);

   // constructors
   Gauge32 g1;
   ACE_ASSERT(g1 == def);
   Gauge32 g2(l);
   ACE_ASSERT(g2 == static_cast<unsigned long> (l));
   Gauge32 g3(nl);
   ACE_ASSERT(g3 == static_cast<unsigned long> (nl));
   Gauge32 g4(ul);
   ACE_ASSERT(g4 == ul);
   Gauge32 g5(i);
   ACE_ASSERT(g5 == static_cast<unsigned long> (i));
   Gauge32 g6(ni);
   ACE_ASSERT(g6 == static_cast<unsigned long> (ni));
   Gauge32 g7(ui);
   ACE_ASSERT(g7 == ui);
   Gauge32 *g8 = new Gauge32(g5);
   ACE_ASSERT(g8 != 0);
   delete g8;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g1(\"\") [%u]\n",
    (unsigned long)g1));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g2(\"%u\") [%u]\n",
    l, (unsigned long)g2));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g3(\"%u\") [%u]\n",
    nl, (unsigned long)g3));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g4(\"%u\") [%u]\n",
    ul, (unsigned long)g4));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g5(\"%u\") [%u]\n",
    i, (unsigned long)g5));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g6(\"%u\") [%u]\n",
    ni, (unsigned long)g6));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) g7(\"%u\") [%u]\n",
    ui, (unsigned long)g7));

  // assignent
  g1 = g2;  // obj
  ACE_ASSERT(g1 == g2);
  g1 = g1; // self
  ACE_ASSERT(g1 == g1);
  g1 = def; // unsigned long
  ACE_ASSERT(g1 == def);
  g1 = us; // unsigned short
  ACE_ASSERT(g1 == static_cast<unsigned long> (us));
  g1 = si; // unsigned short
  ACE_ASSERT(g1 == static_cast<unsigned long> (si));
#endif /*ACE_WIN32*/
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Gauge_Test"));

  TestGuage();

  ACE_END_TEST;
  return 0;
}
