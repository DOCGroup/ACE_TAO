// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    Varbind_Test.cpp
//
// = DESCRIPTION
//  Test all the member functions of the Varbind class.
//   A varbind is a list of { oids and associated values }
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
#include "asnmp/vb.h"
#include "test_config.h"

/*
  Vb( void);
  Vb( const Oid &oid);
  Vb( const Vb &vb);
  Vb( const Oid& vb, const SnmpSyntax &val, const SmiUINT32=SNMP_CLASS_SUCCESS);
  ~Vb();
   int valid() const;
   Vb& operator=( const Vb &vb);
   void set_oid( const Oid& oid);
   void get_oid( Oid &oid) const;
   void set_null();
   void set_value( const TimeTicks& ticks);
   void set_value( const Oid& oid);
   void set_value( const Counter32& ctr);
   void set_value( const Counter64& ctr);
   void set_value( const Gauge32& ctr);
   void set_value( const SnmpUInt32& ctr);
   void set_value( const SnmpInt32& ctr);
   void set_value( const OctetStr& oct_str);
   int get_value( TimeTicks& ticks);
   int get_value( Oid& oid);
   int get_value( Counter32& ctr);
   int get_value( Counter64& ctr);
   int get_value( Gauge32& ctr);
   int get_value( SnmpUInt32& ctr);
   int get_value( SnmpInt32& ctr);
   int get_value( OctetStr& oct_str);
   int get_value( SnmpSyntax &val);
   void set_value( const SnmpSyntax &val);
   int get_value( const SnmpSyntax &val);
   SmiUINT32 get_syntax();
   friend void set_exception_status( Vb *vb, const SmiUINT32 status);
   char *to_string();
   char *to_string_value();
   char *to_string_oid();
 */

static void VbTest()
{
#if !defined (ACE_WIN32)

  Vb v1;
  ACE_ASSERT(v1.valid() == 0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) VarBinad:v1(\"/\") [%s]\n",
    v1.to_string()));

  // purpose of this routine??
  set_exception_status( &v1, 10);

  Vb v2(v1);
  ACE_ASSERT(v2.valid() == 0);
  Oid o1("1.2.3"), o2;
  v2.set_oid(o1);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) VarBinad:v2(\"1.2.3/\") [%s]\n",
    v2.to_string()));

  v2.get_oid(o2);
  ACE_ASSERT(o2 == o1);
  ACE_ASSERT(v2.valid() == 0);
  v2.set_null();
  ACE_ASSERT(v2.valid() == 0);
  v2.get_oid(o2);

  Vb v3;
  TimeTicks t(0), t1;
  v3.set_oid(o1);
  v3.set_value(t);
  ACE_ASSERT(v3.valid() == 1);
  v3.get_value(t1);
  ACE_ASSERT(t == t1);

  Vb v4;
  v4.set_oid(o1);
  v4.set_value(o1);
  ACE_ASSERT(v4.valid() == 1);
  v4.get_value(o2);
  ACE_ASSERT(o1 == o2);

  Vb v5;
  Counter32 c1(12), c2;
  v5.set_oid(o1);
  v5.set_value(c1);
  ACE_ASSERT(v5.valid() == 1);
  v5.get_value(c2);
  ACE_ASSERT(c1 == c2);

  Vb v6;
  Counter64 c3(12345678901234ULL), c4;
  v6.set_oid(o1);
  v6.set_value(c3);
  ACE_ASSERT(v6.valid() == 1);
  v6.get_value(c4);
  ACE_ASSERT(c3 == c4);

  Vb v7;
  Gauge32 g1(0123456), g2;
  v7.set_oid(o1);
  v7.set_value(g1);
  ACE_ASSERT(v7.valid() == 1);
  v7.get_value(g2);
  ACE_ASSERT(g1 == g2);

  Vb v8;
  SnmpInt32 i1(0123456), i2;
  v8.set_oid(o1);
  v8.set_value(i1);
  ACE_ASSERT(v8.valid() == 1);
  v8.get_value(i2);
  ACE_ASSERT(i1 == i2);

  Vb v9;
  SnmpUInt32 u1(0123456), u2;
  v9.set_oid(o1);
  v9.set_value(u1);
  ACE_ASSERT(v9.valid() == 1);
  v9.get_value(u2);
  ACE_ASSERT(u1 == u2);

  Vb v10;
  OctetStr s1(" abcdefghighlmnopqrstuvwxyz!@#$%^&*()"), s2;
  v10.set_oid(o1);
  v10.set_value(s1);
  ACE_ASSERT(v10.valid() == 1);
  v10.get_value(s2);
  ACE_ASSERT(s1 == s2);
  ACE_ASSERT(s1.length() == s2.length());

  // test assignment over all datatypes
  v10 = v5;
  ACE_ASSERT(v10 == v5);


  Vb v11(o1, s1, SNMP_CLASS_SUCCESS);
  ACE_ASSERT(v11.valid() == 1);
  v11.get_oid(o2);
  ACE_ASSERT(o1 == o2);
  v11.get_value(s2);
  ACE_ASSERT(s1 == s2);
#endif /*if ACE_WIN32*/
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Varbind_Test"));
  VbTest();
  ACE_END_TEST;
  return 0;
}

