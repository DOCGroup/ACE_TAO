// -*- C++ -*-

#include "ACEXML/common/Transcode.h"
#include "ACEXML/common/XML_Codecs.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include <array>
#include <cstddef>
#include <cstring>
#include <memory>

void dump_utf16 (ACEXML_UTF16 const *data, size_t len)
{
  for (size_t i {}; i < len; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "%04x", data[i]));

      if (i + 1 == len)
        continue;

      if ((i + 1) % 4 == 0)
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

  size_t x {};

  for (; temp < 0x10000; ++x, temp <<= 1)
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

  size_t encoded_len {};
  std::unique_ptr<ACEXML_Char[]> encoded {
    ACEXML_Base64::encode (ACE_TEXT ("A"), &encoded_len)};
  if (!encoded || encoded_len != 5 ||
      ACE_OS::strcmp (encoded.get (), ACE_TEXT ("QQ==\n")) != 0)
    return 1;

  size_t decoded_len {};
  std::unique_ptr<ACEXML_Char[]> base64_decoded {
    ACEXML_Base64::decode (encoded.get (), &decoded_len)};
  if (!base64_decoded || decoded_len != 1 ||
      base64_decoded[0] != static_cast<ACEXML_Char> ('A'))
    return 1;

  return 0;
}
