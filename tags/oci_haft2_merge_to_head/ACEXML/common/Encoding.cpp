// -*- C++ -*-  $Id$

#include "ACEXML/common/Encoding.h"

const ACEXML_Char* ACEXML_Encoding::encoding_names_[8] = {
  ACE_TEXT ("UCS-4BE"),
  ACE_TEXT ("UCS-4LE"),
  ACE_TEXT ("UCS-4_2143"),
  ACE_TEXT ("UCS-4_3412"),
  ACE_TEXT ("UTF-16"),
  ACE_TEXT ("UTF-16"),
  ACE_TEXT ("UTF-8"),
  ACE_TEXT ("Unsupported Encoding")
};

const ACEXML_UTF8 ACEXML_Encoding::byte_order_mark_[][4] = {
  { '\x00', '\x00', '\xFE', '\xFF' }, // UCS-4, big-endian  (1234 order)
  { '\xFF', '\xFE', '\x00', '\x00' }, // UCS-4, little-endian  (4321 order)
  { '\x00', '\x00', '\xFF', '\xFE' }, // UCS-4, unusual octet order (2143)
  { '\xFE', '\xFF', '\x00', '\x00' }, // UCS-4, unusual octet order (3412)
  { '\xFE', '\xFF', '\xFF', '\xFF' }, // UTF-16, big-endian (3 & 4 != 0)
  { '\xFF', '\xFE', '\xFF', '\xFF' }, // UTF-16, little-endian ( 3 & 4 != 0)
  { '\xEF', '\xBB', '\xBF', '\xFF' }  // UTF-8
};

const ACEXML_UTF8 ACEXML_Encoding::magic_values_[][4] = {
  { '\x00', '\x00', '\x00', '\x3c' }, //
  { '\x3c', '\x00', '\x00', '\x00' }, // UCS-4 and variants
  { '\x00', '\x00', '\x3c', '\x00' }, //
  { '\x00', '\x3c', '\x00', '\x00' }, //
  { '\x00', '\x3c', '\x00', '\x3f' }, // UTF-16BE
  { '\x3c', '\x00', '\x3f', '\x00' }, // UTF-16LE
  { '\x3c', '\x3f', '\x78', '\x6d' }, // UTF-8
};

const ACEXML_Char*
ACEXML_Encoding::get_encoding (const char* input)
{
  if ((ACE_OS::memcmp (&ACEXML_Encoding::byte_order_mark_[ACEXML_Encoding::UTF16BE][0], input, 2) == 0)
      && (input[2] != 0 || input[3] != 0)) // 3 & 4 should not be both zero
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF16BE];
  else if ((ACE_OS::memcmp (&ACEXML_Encoding::byte_order_mark_[ACEXML_Encoding::UTF16LE][0], input, 2) == 0)
    && (input[2] != 0 || input[3] != 0)) // 3 & 4 should not be both zero
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF16LE];
  else if (ACE_OS::memcmp (&ACEXML_Encoding::byte_order_mark_[ACEXML_Encoding::UTF8][0], input, 3) == 0)
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF8];
  else if (ACE_OS::memcmp (&ACEXML_Encoding::magic_values_[ACEXML_Encoding::UTF16BE][0], input, 4) == 0)
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF16BE];
  else if (ACE_OS::memcmp (&ACEXML_Encoding::magic_values_[ACEXML_Encoding::UTF16LE][0], input, 4) == 0)
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF16LE];
  else if (ACE_OS::memcmp (&ACEXML_Encoding::magic_values_[ACEXML_Encoding::UTF8][0], input, 4) == 0)
    return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF8];
  else
    {
      // ACE_ERROR ((LM_ERROR, "Unknown encoding. Assuming UTF-8\n"));
      return ACEXML_Encoding::encoding_names_[ACEXML_Encoding::UTF8];
    }
}
