// -*- C++ -*- $Id$

#include "ACEXML/common/Transcode.h"
#include "ace/Log_Msg.h"

void dump_utf16 (const ACEXML_UTF16 *data,
                 size_t len)
{
  size_t ptr = 0;

  while (1)
    {
      ACE_DEBUG ((LM_DEBUG, "%04x", data[ptr]));

      if (++ptr >= len)
        break;

      if (ptr % 4 == 0)
        ACE_DEBUG ((LM_DEBUG, "\n"));
      else
        ACE_DEBUG ((LM_DEBUG, "  "));
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));
  return;
}

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  ACEXML_UTF16 org [18];
//  =  { 1, 2, 4, 8, 0x10, 0x20, 0x40,
//      0x80,
//      0x100, 0x200, 0x400,
//      0x800, 0x801, 0x802, 0x804, 0x808, 0x810, 0x820,
//      0x840, 0x880, 0x900, 0xa00, 0xc00,
//      0x1000, 0x2000, 0x4000, 0x8000, 0 }
  ACEXML_UCS4 temp = 1;

  ACE_OS::memset (org, 0, sizeof org);
  size_t x;

  for (x = 0; temp < 0x10000; x++, temp <<= 1)
    {
      org[x] = ACE_static_cast (ACEXML_UTF16, temp);
    }

  ACE_DEBUG ((LM_DEBUG, "Original UTF16 string:\n"));
  dump_utf16 (org, x);
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  ACEXML_UTF8 decoded [MAXPATHLEN];
  ACE_OS::memset (decoded, 0, sizeof decoded);

  ACEXML_Transcoder::utf16s2utf8s (org, decoded, MAXPATHLEN);

  ACE_DEBUG ((LM_DEBUG, "Transcoded UTF8 string:\n"));
  ACE_HEX_DUMP ((LM_DEBUG, decoded, ACE_OS::strlen (decoded) + 1));
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  ACEXML_UTF16 after [18];
  ACE_OS::memset (after, 0, sizeof after);
  ACEXML_Transcoder::utf8s2utf16s (decoded, after, 18);

  ACE_DEBUG ((LM_DEBUG, "Restored UTF16 string:\n"));
  dump_utf16 (after, x);
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  return 0;
}
