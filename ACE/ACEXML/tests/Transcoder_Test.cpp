// -*- C++ -*-

#include "ACEXML/common/Transcode.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <array>
#include <cstddef>
#include <cstring>

void dump_utf16 (ACEXML_UTF16 const *data, std::size_t len)
{
  std::size_t ptr {};

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
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::array<ACEXML_UTF16, 18> org {};
  ACEXML_UCS4 temp {1};

  std::size_t x {};

  for (x = 0; temp < 0x10000; x++, temp <<= 1)
    {
      org[x] = static_cast<ACEXML_UTF16> (temp);
    }

  ACE_DEBUG ((LM_DEBUG, "Original UTF16 string:\n"));
  dump_utf16 (org.data (), x);
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  std::array<ACEXML_UTF8, MAXPATHLEN> decoded {};

  ACEXML_Transcoder::utf16s2utf8s (org.data (), decoded.data (), decoded.size ());

  ACE_DEBUG ((LM_DEBUG, "Transcoded UTF8 string:\n"));
  ACE_HEX_DUMP ((LM_DEBUG, decoded.data (), std::strlen (decoded.data ()) + 1));
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  std::array<ACEXML_UTF16, 18> after {};
  ACEXML_Transcoder::utf8s2utf16s (decoded.data (), after.data (), after.size ());

  ACE_DEBUG ((LM_DEBUG, "Restored UTF16 string:\n"));
  dump_utf16 (after.data (), x);
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  return 0;
}
