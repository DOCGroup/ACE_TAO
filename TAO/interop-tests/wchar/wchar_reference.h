// -*- C++ -*-

//=============================================================================
/**
 *  @file    wchar_reference.cpp
 *
 *  $Id$
 *
 *  C++ reference data for testing interoperability with wchars. This is
 *  linked into both clients and servers so that both sides have common
 *  values for testing what is received or sent.
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef WCHAR_REFERENCE_H
#define WCHAR_REFERENCE_H

#define NUM_KEYS 1

#include "ace/OS_NS_wchar.h"

class wchar_reference
{
 public:
  wchar_reference (int v = 0);
  void set_verbose (int v);
  ACE_OS::WChar get_wchar (short key);
  const ACE_OS::WChar *get_wstring (short key);
  ACE_OS::WChar *get_warray (short key);
  void assign_warray (short key, ACE_OS::WChar *warray);

  const ACE_OS::WChar *get_except (short key);

  int match_wchar (short key, ACE_OS::WChar test);
  int match_wstring (short key, const ACE_OS::WChar* test);
  int match_warray (short key, const ACE_OS::WChar *test);
  int match_except (short key, const ACE_OS::WChar *test);

 private:
#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
  static ACE_OS::WChar ref_wchar[NUM_KEYS];
  static const ACE_OS::WChar *ref_wstring[NUM_KEYS];
  static ACE_OS::WChar ref_warray[NUM_KEYS][10];
  static const ACE_OS::WChar *ref_except[NUM_KEYS];
#endif // ACE_HAS_WCHAR
  int verbose_;
};

#endif // WCHAR_REFERENCE_H
