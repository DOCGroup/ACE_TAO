// -*- C++ -*-

//=============================================================================
/**
 *  @file    wchar_reference.cpp
 *
 *  C++ reference data for testing interoperability with wchars. This is
 *  linked into both clients and servers so that both sides have common
 *  values for testing what is received or sent.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#include "wchar_reference.h"
#include "ace/Log_Msg.h"
#include <cstring>
#include <cwchar>

#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)

ACE_OS::WChar const
wchar_reference::ref_wchar[NUM_KEYS] =
  {1234};

ACE_OS::WChar const *
wchar_reference::ref_wstring[NUM_KEYS] =
  {L"have a nice day"};

ACE_OS::WChar
wchar_reference::ref_warray[NUM_KEYS][WARRAY_LENGTH] =
  { {L'a',L'A',L'!',L'1',L'4',L'[',L'?',L'%',L'X',L'E'} };

ACE_OS::WChar const *
wchar_reference::ref_except[NUM_KEYS] =
  {L"TEST EXCEPTION"};

wchar_reference::wchar_reference (bool verbose)
  : verbose_ (verbose)
{
}

void
wchar_reference::set_verbose (bool verbose)
{
  this->verbose_ = verbose;
}

ACE_OS::WChar
wchar_reference::get_wchar (short key) const
{
  return wchar_reference::ref_wchar[key];
}

ACE_OS::WChar const *
wchar_reference::get_wstring (short key) const
{
  return wchar_reference::ref_wstring[key];
}

ACE_OS::WChar *
wchar_reference::get_warray (short key) const
{
  return wchar_reference::ref_warray[key];
}

void
wchar_reference::assign_warray (short key,
                                ACE_OS::WChar *warray) const
{
  std::memcpy (warray, wchar_reference::ref_warray[key], WARRAY_LENGTH * sizeof (ACE_OS::WChar));
}

ACE_OS::WChar const *
wchar_reference::get_except (short key) const
{
  return wchar_reference::ref_except[key];
}

bool
wchar_reference::match_wchar (short key, ACE_OS::WChar test) const
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_wchar, expecting %x, got %x for key %d\n"),
                wchar_reference::ref_wchar[key],test,key));
  return wchar_reference::ref_wchar[key] == test;
}

bool
wchar_reference::match_wstring (short key, ACE_OS::WChar const *test) const
{
  if (key == -1)
    {
      if (verbose_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("match_wstring: expecting nul string, ")
                    ACE_TEXT("got string length %B\n"),
                    std::wcslen (test)));
      return std::wcslen (test) == 0;
    }
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_wstring: expecting %W, got %W for key %d\n"),
                wchar_reference::ref_wstring[key],test,key));
  return std::wcscmp (wchar_reference::ref_wstring[key], test) == 0;
}

bool
wchar_reference::match_warray (short key, ACE_OS::WChar const *test) const
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_warray: key %d\n"),key));
  for (size_t i = 0; i < WARRAY_LENGTH; ++i)
    {
      if (verbose_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("  expecting[%B] %x, got %x\n"),
                    i, wchar_reference::ref_warray[key][i],
                    test[i]));
      if (wchar_reference::ref_warray[key][i] != test[i])
        return false;
    }
  return true;
}

bool
wchar_reference::match_except (short key, ACE_OS::WChar const *test) const
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_except: expecting %W, got %W for key %d\n"),
                wchar_reference::ref_except[key],test,key));
  return std::wcscmp (wchar_reference::ref_except[key], test) == 0;
}
#else // ACE_HAS_WCHAR
wchar_reference::wchar_reference (bool verbose)
  : verbose_ (verbose)
{
}

void
wchar_reference::set_verbose (bool verbose)
{
  this->verbose_ = verbose;
}

ACE_OS::WChar
wchar_reference::get_wchar (short) const
{
  return {};
}

ACE_OS::WChar const *
wchar_reference::get_wstring (short) const
{
  return {};
}

ACE_OS::WChar *
wchar_reference::get_warray (short) const
{
  return {};
}

void
wchar_reference::assign_warray (short, ACE_OS::WChar *) const
{
}

ACE_OS::WChar const *
wchar_reference::get_except (short) const
{
  return {};
}

bool
wchar_reference::match_wchar (short, ACE_OS::WChar) const
{
  return false;
}

bool
wchar_reference::match_wstring (short, ACE_OS::WChar const *) const
{
  return false;
}

bool
wchar_reference::match_warray (short, ACE_OS::WChar const *) const
{
  return false;
}

bool
wchar_reference::match_except (short, ACE_OS::WChar const *) const
{
  return false;
}

#endif // ACE_HAS_WCHAR
