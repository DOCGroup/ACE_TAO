// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    interop_test/wchar
//
// = FILENAME
//    wchar_reference.cpp
//
// = DESCRIPTION
//    C++ reference data for testing interoperability with wchars. This is
//    linked into both clients and servers so that both sides have common
//    values for testing what is received or sent.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================
#ifndef WCHAR_REFERENCE_H
#define WCHAR_REFERENCE_H

#include "ace/OS.h"

#define NUM_KEYS 1

class wchar_reference
{
 public:
  wchar_reference (int v = 0);
  void set_verbose (int v);
  wchar_t get_wchar (short key);
  const wchar_t *get_wstring (short key);
  wchar_t *get_warray (short key);
  void assign_warray (short key, wchar_t *warray);

  const wchar_t *get_except (short key);

  int match_wchar (short key, wchar_t test);
  int match_wstring (short key, const wchar_t* test);
  int match_warray (short key, const wchar_t *test);
  int match_except (short key, const wchar_t *test);

 private:
#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
  static wchar_t ref_wchar[NUM_KEYS];
  static const wchar_t *ref_wstring[NUM_KEYS];
  static wchar_t ref_warray[NUM_KEYS][10];
  static const wchar_t *ref_except[NUM_KEYS];
#endif // ACE_HAS_WCHAR
  int verbose_;
};

#endif // WCHAR_REFERENCE_H
