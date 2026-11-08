// -*- C++ -*-

//=============================================================================
/**
 *  @file    wchar_reference.h
 *
 *  C++ reference data for testing interoperability with wchars. This is
 *  linked into both clients and servers so that both sides have common
 *  values for testing what is received or sent.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef WCHAR_REFERENCE_H
#define WCHAR_REFERENCE_H

#include <cstddef>

constexpr std::size_t NUM_KEYS = 1;
constexpr std::size_t WARRAY_LENGTH = 10;

#include "ace/OS_NS_wchar.h"

class wchar_reference
{
 public:
  explicit wchar_reference (bool verbose = false);
  void set_verbose (bool verbose);
  ACE_OS::WChar get_wchar (short key) const;
  ACE_OS::WChar const *get_wstring (short key) const;
  ACE_OS::WChar *get_warray (short key) const;
  void assign_warray (short key, ACE_OS::WChar *warray) const;

  ACE_OS::WChar const *get_except (short key) const;

  bool match_wchar (short key, ACE_OS::WChar test) const;
  bool match_wstring (short key, ACE_OS::WChar const *test) const;
  bool match_warray (short key, ACE_OS::WChar const *test) const;
  bool match_except (short key, ACE_OS::WChar const *test) const;

 private:
#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
  static ACE_OS::WChar const ref_wchar[NUM_KEYS];
  static ACE_OS::WChar const *ref_wstring[NUM_KEYS];
  static ACE_OS::WChar ref_warray[NUM_KEYS][WARRAY_LENGTH];
  static ACE_OS::WChar const *ref_except[NUM_KEYS];
#endif // ACE_HAS_WCHAR
  bool verbose_ {};
};

#endif // WCHAR_REFERENCE_H
