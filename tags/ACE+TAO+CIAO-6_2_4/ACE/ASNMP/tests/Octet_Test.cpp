
//=============================================================================
/**
 *  @file    Octet_Test.cpp
 *
 *  $Id$
 *
 *  Test all the member functions of the OCTET class. An Object
 *  representing an ASN.1 Integer64 SMI OCTET STRING SYNTAX.
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
#include "asnmp/octet.h"
#include "test_config.h"

/*
  OctetStr( const char *string, long size = -1);
  OctetStr ( const OctetStr &octet);
  ~OctetStr();

  SmiUINT32 get_syntax();
  void set_data( const SmiBYTE* string, long int size = -1);
  OctetStr& operator=( const char *string);
  OctetStr& operator=( const OctetStr &octet);

  bool operator==( const OctetStr &lhs, const OctetStr &rhs);
  bool operator!=( const OctetStr &lhs, const OctetStr &rhs);
  bool operator<( const OctetStr &lhs, const OctetStr &rhs);
  bool operator<=( const OctetStr &lhs,const OctetStr &rhs);
  bool operator>( const OctetStr &lhs, const OctetStr &rhs);
  bool operator>=( const OctetStr &lhs, const OctetStr &rhs);
  bool operator==( const OctetStr &lhs,const char *rhs);
  bool operator!=( const OctetStr &lhs,const char  *rhs);
  bool operator<( const OctetStr &lhs,const char  *rhs);
  bool operator<=( const OctetStr &lhs,char  *rhs);
  bool operator>( const OctetStr &lhs,const char  *rhs);
  bool operator>=( const OctetStr &lhs,const char  *rhs);
  OctetStr& operator+=( const SmiBYTE  *a);
  OctetStr& operator+=( const char c);
  OctetStr& operator+=( const OctetStr& octetstr);
  SmiBYTE& operator[]( int position);
  int nCompare( const long n, const OctetStr &o) const;
  size_t length() const ;
  int valid() const;
  SmiBYTE *data() const;
  char *to_string();
  char *to_string_hex();
  SnmpSyntax  *clone() const;
  SnmpSyntax& operator=( SnmpSyntax &val);
 */

static void TestOctet()
{
  const char *str = "A test of octet strings...!@@#$%^&*()_+|~{}:,./<>?";
  OctetStr o1;
  ACE_ASSERT(o1.valid() == 1);
  ACE_ASSERT(o1.length() == 0);
  ACE_ASSERT(o1.data() != (unsigned char *)0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o1(\"\") [%s]\n",
    o1.to_string()));
  o1.set_data((SmiBYTE *)str);
  ACE_ASSERT(!ACE_OS::strcmp(str, (char *)o1.data()));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o1(\"str\") [%s]\n",
    o1.to_string()));

  OctetStr o2(str);
  ACE_ASSERT(o2.valid() == 1);
  ACE_ASSERT(o2.data() != (unsigned char *)0);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o2(\"str\") [%s]\n",
    o2.to_string()));

  OctetStr o3(str, 4);  // test setting less than full string length
  ACE_ASSERT(o3.valid() == 1);
  ACE_ASSERT(o3.length() == 4);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o3(\"A te\") [%s]\n",
    o3.to_string()));

  OctetStr o4(o3);  // test setting less than full string length
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o4(\"A te\") [%s]\n",
    o4.to_string()));
  ACE_ASSERT(o4.valid() == 1);
  ACE_ASSERT(o4.length() == 4);

  OctetStr o5;
  o5 = str;
  ACE_ASSERT(o5.valid() == 1);
  ACE_ASSERT(o5.length() == ACE_OS::strlen(str));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o5(\"str\") [%s]\n",
    o5.to_string()));

  OctetStr o6;
  o6 = o5;
  ACE_ASSERT(o6.valid() == 1);
  ACE_ASSERT(o5.length() == o6.length());
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o6(\"str\") [%s]\n",
    o6.to_string()));

  o6 += o3;
  o6 = "";
  o6 += str;
  o6 += '#';
  ACE_ASSERT(o6[0] == (SmiBYTE) 'A');
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Octet:o6(\"str\") [%s]\n",
    o6.to_string()));

   ACE_ASSERT(!(o3 < o3));
   ACE_ASSERT(!(o3 > o3));
   ACE_ASSERT(o3 >= o3);
   ACE_ASSERT(o3 <= o3);
   ACE_ASSERT(o3 == o3);
   ACE_ASSERT(!(o3 != o3));

}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Octet_Test"));

  TestOctet();
  ACE_END_TEST;
  return 0;
}
