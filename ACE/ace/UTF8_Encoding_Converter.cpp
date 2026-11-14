#include "ace/UTF8_Encoding_Converter.h"

#include <cstring>

#if defined (ACE_USES_WCHAR)
#include "ace/UTF16_Encoding_Converter.h"
#include "ace/UTF32_Encoding_Converter.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_UTF8_Encoding_Converter::ACE_UTF8_Encoding_Converter ()
{
  // Choose a converter for the ASCII or UTF-8 string to a wide character
  // string which we will use in from_utf8.  We have to make an
  // assumption here about the encoding based on the size of ACE_TCHAR.
  ACE_Encoding_Converter *native {};
  switch (sizeof (ACE_TCHAR))
    {
    case 4:
      ACE_NEW (native, ACE_UTF32_Encoding_Converter);
      break;
    case 2:
      ACE_NEW (native, ACE_UTF16_Encoding_Converter);
      break;
    }
  this->native_.reset (native);
}

ACE_UTF8_Encoding_Converter::~ACE_UTF8_Encoding_Converter ()
  = default;

ACE_UTF8_Encoding_Converter::Result
ACE_UTF8_Encoding_Converter::to_utf8 (void const *source,
                                      size_t source_size,
                                      ACE_Byte *target,
                                      size_t target_size,
                                      bool /*strict*/)
{
  if (target_size >= source_size)
    {
      std::memcpy (target, source, source_size);
      return CONVERSION_OK;
    }

  return TARGET_EXHAUSTED;
}

ACE_UTF8_Encoding_Converter::Result
ACE_UTF8_Encoding_Converter::from_utf8 (ACE_Byte const *source,
                                        size_t source_size,
                                        void *target,
                                        size_t target_size,
                                        bool strict)
{
  if (this->native_ != nullptr)
    {
      return this->native_->from_utf8 (source, source_size,
                                       target, target_size, strict);
    }

  ACE_TCHAR *target_start = static_cast<ACE_TCHAR *> (target);
  ACE_OS::strncpy (target_start,
                   ACE_TEXT_CHAR_TO_TCHAR (
                     reinterpret_cast<char const *> (source)),
                   source_size);
  target_start[source_size] = 0;
  return CONVERSION_OK;
}

ACE_UTF8_Encoding_Converter *
ACE_UTF8_Encoding_Converter::encoded (ACE_Byte const *source,
                                      size_t source_size)
{
  for (size_t i = 0; i < source_size; ++i)
    {
      if (source[i] < 0x01 || source[i] > 0x7f)
        return nullptr;
    }

  // All characters are "valid" ASCII
  ACE_UTF8_Encoding_Converter *converter {};
  ACE_NEW_RETURN (converter,
                  ACE_UTF8_Encoding_Converter,
                  nullptr);
  return converter;
}

ACE_END_VERSIONED_NAMESPACE_DECL
#endif /* ACE_USES_WCHAR */
