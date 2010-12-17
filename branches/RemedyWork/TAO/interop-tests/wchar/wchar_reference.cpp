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
#include "wchar_reference.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)

ACE_OS::WChar
wchar_reference::ref_wchar[NUM_KEYS] =
  {1234};

const ACE_OS::WChar *
wchar_reference::ref_wstring[NUM_KEYS] =
  {L"have a nice day"};

ACE_OS::WChar
wchar_reference::ref_warray[NUM_KEYS][10] =
  { {L'a',L'A',L'!',L'1',L'4',L'[',L'?',L'%',L'X',L'E'} };

const ACE_OS::WChar *
wchar_reference::ref_except[NUM_KEYS] =
  {L"TEST EXCEPTION"};

wchar_reference::wchar_reference(int v)
  : verbose_ (v)
{
}

void
wchar_reference::set_verbose(int v)
{
  this->verbose_ = v;
}

ACE_OS::WChar
wchar_reference::get_wchar (short key)
{
  return wchar_reference::ref_wchar[key];
}

const ACE_OS::WChar *
wchar_reference::get_wstring (short key)
{
  return wchar_reference::ref_wstring[key];
}

ACE_OS::WChar *
wchar_reference::get_warray (short key)
{
  return wchar_reference::ref_warray[key];
}

void
wchar_reference::assign_warray (short key,
                                ACE_OS::WChar *warray)
{
  ACE_OS::memcpy(warray,
                 wchar_reference::ref_warray[key],
                 10*(sizeof(ACE_OS::WChar))
                 );
}

const ACE_OS::WChar *
wchar_reference::get_except (short key)
{
  return wchar_reference::ref_except[key];
}

int
wchar_reference::match_wchar (short key, ACE_OS::WChar test)
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_wchar, expecting %x, got %x for key %d\n"),
                wchar_reference::ref_wchar[key],test,key));
  return wchar_reference::ref_wchar[key] == test;
}

int
wchar_reference::match_wstring (short key, const ACE_OS::WChar *test)
{
  if (key == -1)
    {
      if (verbose_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("match_wstring: expecting nul string, ")
                    ACE_TEXT("got string length %d\n"),
                    ACE_OS::strlen (test)));
      return (ACE_OS::strlen (test) == 0);
    }
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_wstring: expecting %W, got %W for key %d\n"),
                wchar_reference::ref_wstring[key],test,key));
  return ACE_OS::strcmp(wchar_reference::ref_wstring[key],test) == 0;
}

int
wchar_reference::match_warray (short key, const ACE_OS::WChar *test)
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_warray: key %d\n"),key));
  for (int i = 0; i < 10; i++)
    {
      if (verbose_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("  expecting[%d] %x, got %x\n"),
                    i, wchar_reference::ref_warray[key][i],
                    test[i]));
      if (wchar_reference::ref_warray[key][i] != test[i])
        return 0;
    }
  return 1;
}

int
wchar_reference::match_except(short key, const ACE_OS::WChar *test)
{
  if (verbose_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("match_except: expecting %W, got %W for key %d\n"),
                wchar_reference::ref_except[key],test,key));
  return ACE_OS::strcmp(wchar_reference::ref_except[key],test) == 0;
}
#else // ACE_HAS_WCHAR
wchar_reference::wchar_reference(int v)
  : verbose_ (v)
{
}

void
wchar_reference::set_verbose(int v)
{
  this->verbose_ = v;
}

ACE_OS::WChar
wchar_reference::get_wchar (short )
{
  return 0;
}

const ACE_OS::WChar *
wchar_reference::get_wstring (short )
{
  return 0;
}

ACE_OS::WChar *
wchar_reference::get_warray (short )
{
  return 0;
}

void
wchar_reference::assign_warray (short ,
                                ACE_OS::WChar *)
{
}

const ACE_OS::WChar *
wchar_reference::get_except (short )
{
  return 0;
}

int
wchar_reference::match_wchar (short , ACE_OS::WChar )
{
  return 0;
}

int
wchar_reference::match_wstring (short , const ACE_OS::WChar *)
{
  return 0;
}

int
wchar_reference::match_warray (short , const ACE_OS::WChar *)
{
  return 0;
}

#endif // ACE_HAS_WCHAR
