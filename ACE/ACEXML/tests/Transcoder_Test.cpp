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
      {
        continue;
      }

      if ((i + 1) % 4 == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      else
      {
        ACE_DEBUG ((LM_DEBUG, "  "));
      }
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

  for (size_t i {}; i < x; ++i)
    {
      if (after[i] != org[i])
        {
          return 1;
        }
    }

  std::array<ACEXML_UTF8, 2> boundary {};
  if (ACEXML_Transcoder::utf162utf8 (0x7ff, boundary.data (), boundary.size ()) != 2 ||
      static_cast<unsigned char> (boundary[0]) != 0xdf ||
      static_cast<unsigned char> (boundary[1]) != 0xbf)
    {
      return 1;
    }

  std::array<ACEXML_UCS4, 3> const scalars {{0x10000, 0x1f600, 0x10ffff}};
  for (ACEXML_UCS4 const scalar : scalars)
    {
      std::array<ACEXML_UTF16, 2> pair {};
      std::array<ACEXML_UTF8, 4> encoded_scalar {};
      std::array<ACEXML_UTF8, 4> encoded_pair {};
      ACEXML_UCS4 decoded_scalar {};
      if (ACEXML_Transcoder::ucs42utf16 (scalar, pair.data (), pair.size ()) != 2 ||
          ACEXML_Transcoder::utf162ucs4 (pair.data (), pair.size (), decoded_scalar) != 2 ||
          decoded_scalar != scalar ||
          ACEXML_Transcoder::ucs42utf8 (scalar, encoded_scalar.data (), encoded_scalar.size ()) != 4 ||
          ACEXML_Transcoder::surrogate2utf8 (pair[0], pair[1], encoded_pair.data (), encoded_pair.size ()) != 4 ||
          encoded_pair != encoded_scalar ||
          ACEXML_Transcoder::utf82ucs4 (encoded_scalar.data (), encoded_scalar.size (), decoded_scalar) != 4 ||
          decoded_scalar != scalar)
        {
          return 1;
        }
    }

  ACEXML_UCS4 invalid_scalar {};
  std::array<ACEXML_UTF8, 4> face {};
  if (ACEXML_Transcoder::utf162ucs4 (org.data (), 0, invalid_scalar) != ACEXML_Transcoder::ACEXML_END_OF_SOURCE ||
      ACEXML_Transcoder::surrogate2ucs4 (0xd800, 0x0041, invalid_scalar) != ACEXML_Transcoder::ACEXML_INVALID_ARGS ||
      ACEXML_Transcoder::surrogate2utf8 (0xd83d, 0xde00, face.data (), 3) != ACEXML_Transcoder::ACEXML_DESTINATION_TOO_SHORT ||
      ACEXML_Transcoder::surrogate2utf8 (0xd83d, 0xde00, face.data (), face.size ()) != 4 ||
      static_cast<unsigned char> (face[0]) != 0xf0 ||
      static_cast<unsigned char> (face[1]) != 0x9f ||
      static_cast<unsigned char> (face[2]) != 0x98 ||
      static_cast<unsigned char> (face[3]) != 0x80)
    {
      return 1;
    }

  std::array<std::array<unsigned char, 4>, 5> const malformed {{
    {{0xc0, 0x80, 0, 0}},
    {{0xe0, 0x80, 0x80, 0}},
    {{0xed, 0xa0, 0x80, 0}},
    {{0xf4, 0x90, 0x80, 0x80}},
    {{0xe2, 0x28, 0xa1, 0}}
  }};
  for (std::array<unsigned char, 4> const &bytes : malformed)
    {
      ACEXML_UCS4 result = 0x1234;
      if (ACEXML_Transcoder::utf82ucs4 (
            reinterpret_cast<ACEXML_UTF8 const *> (bytes.data ()),
            bytes.size (), result) != ACEXML_Transcoder::ACEXML_NON_UNICODE ||
          result != 0x1234)
        {
          return 1;
        }
    }
  if (ACEXML_Transcoder::utf82ucs4 (face.data (), 3, invalid_scalar) !=
      ACEXML_Transcoder::ACEXML_END_OF_SOURCE)
    {
      return 1;
    }

  std::array<ACEXML_UTF16, 5> const supplementary {{'A', 0xd83d, 0xde00, 'B', 0}};
  std::array<ACEXML_UTF8, 7> supplementary_utf8 {};
  std::array<ACEXML_UTF16, 5> supplementary_roundtrip {};
  if (ACEXML_Transcoder::utf16s2utf8s (supplementary.data (), supplementary_utf8.data (),
                                        supplementary_utf8.size ()) != 7 ||
      ACEXML_Transcoder::utf8s2utf16s (supplementary_utf8.data (), supplementary_roundtrip.data (),
                                        supplementary_roundtrip.size ()) != 5 ||
      supplementary_roundtrip != supplementary ||
      ACEXML_Transcoder::utf16s2utf8s (supplementary.data (), supplementary_utf8.data (), 5) !=
        ACEXML_Transcoder::ACEXML_DESTINATION_TOO_SHORT ||
      ACEXML_Transcoder::utf8s2utf16s ("A\xf0\x9f\x98\x80" "B", supplementary_roundtrip.data (), 2) !=
        ACEXML_Transcoder::ACEXML_DESTINATION_TOO_SHORT)
    {
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Restored UTF16 string:\n"));
  dump_utf16 (after.data (), x);
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  size_t encoded_len {};
  std::unique_ptr<ACEXML_Char[]> const encoded {
    ACEXML_Base64::encode (ACE_TEXT ("A"), &encoded_len)};
  if (!encoded || encoded_len != 5 ||
      ACE_OS::strcmp (encoded.get (), ACE_TEXT ("QQ==\n")) != 0)
    {
      return 1;
    }

  size_t decoded_len {};
  std::unique_ptr<ACEXML_Char[]> const base64_decoded {
    ACEXML_Base64::decode (encoded.get (), &decoded_len)};
  if (!base64_decoded || decoded_len != 1 ||
      base64_decoded[0] != static_cast<ACEXML_Char> ('A'))
  {
    return 1;
  }

  return 0;
}
