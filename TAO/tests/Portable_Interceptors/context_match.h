#ifndef SERVICE_CONTEXT_MATCH_H
#define SERVICE_CONTEXT_MATCH_H

#include "tao/OctetSeqC.h"
#include <cstring>

inline bool
context_matches (CORBA::OctetSeq const &data, char const *expected)
{
  size_t const length = std::strlen (expected) + 1;
  return data.length () == length
    && std::memcmp (data.get_buffer (), expected, length) == 0;
}

#endif
