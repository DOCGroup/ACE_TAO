
//=============================================================================
/**
 *  @file    Counter64_Test.cpp
 *
 *  $Id$
 *
 *  Test all the member functions of the Counter64 class. An Object
 *  representing an ASN.1 Counter64 SMI 64 bit Integer SYNTAX.
 * (SNMPv2c)
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
#include "asnmp/ctr64.h"
#include "test_config.h"

// TODO: verify this with ACE folks
#if defined(_WIN32)
#define LLONG __int64
#define ULLONG unsigned __int64
#else
#define LLONG long long
#define ULLONG unsigned long long
#endif

/*
     Counter64( unsigned long long llw = 0);
     Counter64( unsigned long hiparm, unsigned long loparm);
     Counter64( const Counter64 &ctr64);
     ~Counter64();
     SmiUINT32 get_syntax();
     long double to_long_double() const;
     Counter64& assign( long double ld);
     unsigned long high() const;
     unsigned long low() const;
     void set_high( const unsigned long h);
     void set_low( const unsigned long l);
     Counter64& operator=( const unsigned long long rhs);
     Counter64& operator=( const Counter64 &rhs);
     char *to_string();
     SnmpSyntax *clone() const;
     SnmpSyntax& operator=( SnmpSyntax &val);
     int valid() const;
     operator unsigned long long();
 */

static void TestCounter64()
{
#if !defined (ACE_WIN32)
  static unsigned long ul = ULONG_MAX;
  LLONG ll =  (LLONG) 0x7fffffffffffffffLL;
  LLONG mll =  (LLONG) ((-ll) - 1);
  ULLONG ull =  (ULLONG) 0xffffffffffffffffULL;
  long double ld = (LLONG) ll;

  cerr << "max unsigned long long is " << ull << endl;
  cerr << "max long long is " << ll << endl;
  cerr << "min long long is " << mll << endl;

  Counter64  c1;
  ACE_ASSERT(c1.valid() == 1);
  Counter64  c2(ul, ul);
  ACE_ASSERT(c2.valid() == 1);
  ACE_ASSERT(c2.high() == ul);
  ACE_ASSERT(c2.low() == ul);

  Counter64  c3(ul);
  ACE_ASSERT(c3.valid() == 1);
  ACE_ASSERT(c3.low() == ul);

  Counter64  c4(c2);
  ACE_ASSERT(c4.valid() == 1);
  ACE_ASSERT(c4.high() == ul);
  ACE_ASSERT(c4.low() == ul);

  Counter64  c5(0);
  ACE_ASSERT(c5.valid() == 1);

  Counter64 c6;
  c6.assign(ld);
  ACE_ASSERT(ACE::is_equal(c6.to_long_double(), ld));

  Counter64 c7(ull);
  ACE_ASSERT(c7 == ull);


  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c1(\"\") [%s]\n",
    c1.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c2(LONG_MAX,LONG_MAX) [%s]\n",
    c2.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c3(LONG_MAX) [%s]\n",
    c3.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c4(c2) [%s]\n",
    c4.to_string()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) c5(0) [%s]\n",
    c5.to_string()));

  // misc routines
   c1.set_low(1);
   c1.set_high(2);
   ACE_ASSERT(c1.low() == 1);
   ACE_ASSERT(c1.high() == 2);
  // assignment
  c5 = c4;
  ACE_ASSERT(c5 == c4);
  c4 = c4;
  ACE_ASSERT(c5 == c4);
  c5 = ll;
  ACE_ASSERT(c5 == static_cast<ACE_UINT64> (ll));
  // try simple arithmetic (needs more test cases)
  c5 = mll;
  c5 = c5 + (ULLONG) 10;
  ACE_ASSERT(c5 == static_cast<ACE_UINT64> (mll + 10));
#endif /*ACE_WIN32 */
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Counter64_Test"));

  TestCounter64();

  ACE_END_TEST;
  return 0;
}
