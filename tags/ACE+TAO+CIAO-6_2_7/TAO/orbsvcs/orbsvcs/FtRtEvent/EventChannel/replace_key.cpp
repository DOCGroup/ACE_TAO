// $Id$

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "tao/Object_KeyC.h"
#include <algorithm>

/// This function was defined in IOGR_Maker.cpp. It is moved
/// here because the use of <algorithm> header
/// file conflicts with the use of auto_ptr in the
/// VxWorks5.5.1_PPC85XX_DIAB platform.

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void replace_key(char* ior, char* end_ior,
                 const TAO::ObjectKey& oldkey,
                 const TAO::ObjectKey& newkey)
{
  size_t keylen = oldkey.length();
  ACE_ASSERT(keylen == newkey.length());

  char* pos = ior;
  const char* oldkey_begin = (const char*)oldkey.get_buffer();
  const char* oldkey_end = oldkey_begin + keylen;

  while ((pos = std::search(pos, end_ior,oldkey_begin, oldkey_end)) != end_ior) {
    ACE_OS::memcpy(pos, newkey.get_buffer(), keylen);
    pos+= keylen;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
