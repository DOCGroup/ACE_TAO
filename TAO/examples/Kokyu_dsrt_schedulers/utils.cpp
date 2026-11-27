#include "utils.h"
#include "Kokyu/Kokyu.h"
#include "ace/Lock_Adapter_T.h"
#include <cstring>

namespace
{
  template <typename Sequence>
  size_t
  guid_to_size_t_i (Sequence const &guid)
  {
    size_t result {};
    size_t const length = guid.length () < sizeof (result)
      ? guid.length () : sizeof (result);
    if (length != 0)
      {
        std::memcpy (&result, guid.get_buffer (), length);
      }
    return result;
  }
}

size_t
guid_to_size_t (Kokyu::GuidType const &guid)
{
  return guid_to_size_t_i (guid);
}

size_t
guid_to_size_t (RTScheduling::Current::IdType const &guid)
{
  return guid_to_size_t_i (guid);
}

void
guid_copy (Kokyu::GuidType &lhs, RTScheduling::Current::IdType const &rhs)
{
  lhs.length (rhs.length ());
  if (rhs.length () != 0)
    {
      std::memcpy (lhs.get_buffer (), rhs.get_buffer (), rhs.length ());
    }
}

void
guid_copy (RTScheduling::Current::IdType &lhs, Kokyu::GuidType const &rhs)
{
  lhs.length (rhs.length ());
  if (rhs.length () != 0)
    {
      std::memcpy (lhs.get_buffer (), rhs.get_buffer (), rhs.length ());
    }
}

